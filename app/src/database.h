/*
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.

   Author: Fredrik Fjeldvær (fredrik@ffj.no)
*/

#ifndef APOTEKH_DATABASE_H
#define APOTEKH_DATABASE_H

#include <QString>
#include <QtSql>
#include <algorithm>
#include <vector>
#include "fest_xml/fest_reader.h"
#include "fmd/fmd_decoder.h"
#include "fmd/fmd_xml_reader.h"
#include "product.h"
#include "error_messages.h"
#include "check_numbers.h"
#include "constants.h"

namespace apotek {
namespace database{

// TODO: make an own class for handling searches (?)

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
    inline std::vector<Product> search_product(const QString& search_product);
    inline bool save_product(const Product& product) const;
    QSqlError save_durability(const Product& product);
    QSqlError remove_durability(const Product& product);
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
    QSqlError update_durability_product(const Product& product);
    std::vector<Product> get_from_xml();
    std::vector<Product> get_newproducts() const;
    QSqlError update_durability();
    QString set_db_path();
    bool check_if_product_exists(int varenr) const;
    bool check_if_durability_exists(int varenr) const;
    bool check_if_newproduct_exists(const Product& product) const;
    std::vector<Product> varenr_search_product(const QString& search_product) const;
    std::vector<Product> ean_search_product(const QString& search_product) const;
    std::vector<Product> navn_search_product(const QString& search_product) const;
    std::vector<Product> datamatrix_search_product(const QString& search_product);
    // data members
    apotek::xml::FEST_Reader m_festreader;
    apotek::xml::FEST_Reader m_festreader_vet;
    QSqlDatabase m_db_file;
    QSqlDatabase m_db_memory;
    std::vector<Product> m_durability_products;
    std::vector<Product> m_products;
    QString m_error_status;
};

// adds a single Vare if it does not already exists and saves it to the varer table in the db.
inline bool Database::save_product(const Product& product) const
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
inline std::vector<Product> Database::search_product(const QString& search_product)
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
        if(search_product.length() > 1 && search_product.length() <= apotek::constants::size_of_ean) {
            result = navn_search_product(search_product);
        } else {
            // it must be data matrix
            result = datamatrix_search_product(search_product);
        }
    }
    return result;
}

} // namespace
} // namespace
#endif // APOTEKH_DATABASE_H
