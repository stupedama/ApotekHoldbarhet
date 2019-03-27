#ifndef APOTEKH_DATABASE_H
#define APOTEKH_DATABASE_H

#include <QString>
#include <QtSql>
#include <algorithm>
#include <vector>
#include "product.h"
#include "error_messages.h"
#include "check_numbers.h"
#include "constants.h"
#include <fest_reader.h>

namespace apotek {
namespace database{

/**
 * @brief The Database class
 * Handles the database connection to SQlite QT handler.
 * It calls the FEST_Reader class to get all the content we want from the
 * fest_xml file.
 *
 * It also handles all the searching of the std::vector<Products> (both m_products and m_durability_products).
 */
class Database
{
public:
    explicit Database();
    ~Database();
    inline std::vector<Product> search_product(const QString& search_product) const;
    inline bool add_product_from_xml(const Product& product) const;
    QSqlError save_durability(const Product& product);
    QSqlError remove_durability(const Product& product);
    QSqlError update_durability();
    QSqlError update_products();
    bool save_newproduct(const Product& product);
    bool compare_fest_hentetdato();
    void init_db();
    void init_db_memory();
    void init_db_file();
    // setters and getters
    std::vector<Product> get_durability() { QSqlError err = update_durability(); return m_durability_products; }
    std::vector<Product> get_products();
    void set_error_status(const QString& e) { m_error_status = e; }
    void set_fest_hentetdato();
    QString get_error_status() { return m_error_status; }
    QString get_fest_hentetdato();
private:
    // private functions
    QSqlError update_durability_product(const Product& product);
    std::vector<Product> get_from_xml();
    QString set_db_path();
    bool check_if_product_exists(int varenr) const;
    bool check_if_durability_exists(int varenr) const;
    bool check_if_newproduct_exists(const Product& product) const;
    std::vector<Product> varenr_search_product(const QString& search_product) const;
    std::vector<Product> ean_search_product(const QString& search_product) const;
    std::vector<Product> navn_search_product(const QString& search_product) const;
    // data members
    apotek::xml::FEST_Reader m_festreader;
    QSqlDatabase m_db_file;
    QSqlDatabase m_db_memory;
    std::vector<Product> m_durability_products;
    std::vector<Product> m_products;
    QString m_error_status;
};

// adds a single Vare if it does not already exists and saves it to the varer table in the db.
inline bool Database::add_product_from_xml(const Product& product) const
{
    QSqlQuery q(m_db_memory);
    const int s_varenr = product.get_varenr();

    if(!check_if_product_exists(s_varenr)) {

        if(!q.prepare(QLatin1String("insert into products(varenr, navn, id, ean, legemiddelform, mengde) values (?, ?, ?, ?, ?, ?)")))
            return true; // q.lastError();

        const int varenr{product.get_varenr()};
        const QString navn{product.get_navn()};
        const QString id{product.get_id()};
        const QString ean{product.get_ean()};
        const QString legemiddelform{product.get_legemiddelform()};
        const int mengde{product.get_mengde()};

        q.addBindValue(varenr);
        q.addBindValue(navn);
        q.addBindValue(id);
        q.addBindValue(ean);
        q.addBindValue(legemiddelform);
        q.addBindValue(mengde);
        q.exec();

        return false;
    }
    return true;
}

// searches all the products in the m_products vector.
inline std::vector<Product> Database::search_product(const QString& search_product) const
{
    std::vector<Product> result;

    if(apotek::apotekholdbarhet::check_numbers(search_product)) {
            if(search_product.length() <= apotek::constants::size_of_varenr) {
                result = varenr_search_product(search_product);
            } else {
                result = ean_search_product(search_product);
            }
    } else {
        // if the string only contains one character, the search will be to heavy.
        // set it minimum 2.
        if(search_product.length() > 1) {
            result = navn_search_product(search_product);
        }
    }
    return result;
}

} // namespace
} // namespace
#endif // APOTEKH_DATABASE_H
