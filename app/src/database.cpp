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

#include "database.h"
#include "database_init.h"

#ifdef _WIN32
    #include <ShlObj.h>
#endif

namespace apotek {
namespace database {

Database::Database() : m_festreader_vet{"./fest/fest251_vet.xml"}
{

    if (!QSqlDatabase::drivers().contains("QSQLITE"))
        set_error_status(apotek::errors::error_database_init);

    apotek::database::database_init(*this);
}


Database::~Database()
{
    m_db_file.close();
    m_db_memory.close();

    QSqlDatabase::removeDatabase("memory");
    QSqlDatabase::removeDatabase("file");
}

QString Database::set_db_path()
{
#ifdef _WIN32
    QString path;
    std::wstring wstr;

    PWSTR pw_path = NULL;
    HRESULT hr = SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, NULL, &pw_path);

    if(SUCCEEDED(hr)) {
		wstr = pw_path;
		path = QString::fromStdWString(wstr);
		path = path + "\\varer.db";
    } else {
        path = "varer.db";
    }

    CoTaskMemFree(static_cast<void*>(pw_path));
    return path;
#else
    return "varer.db";
#endif
}

QSqlError Database::update_durability_product(const Product& product)
{
    QSqlQuery q(m_db_file);
    const int varenr = product.get_varenr();

    if(check_if_durability_exists(varenr)) {
        q.prepare("UPDATE durability_products set lokasjon=:lokasjon, holdbarhet=:holdbarhet WHERE varenr=:varenr");
        QString h = product.get_holdbarhet();
        QString l = product.get_lokasjon();
        int vnr = product.get_varenr();

        q.bindValue(":holdbarhet", h);
        q.bindValue(":lokasjon", l);
        q.bindValue(":varenr", vnr);
        q.exec();
    }
    return QSqlError();
}


void Database::init_db_file()
{
    m_db_file = QSqlDatabase::addDatabase("QSQLITE", "file");

    QString path = set_db_path();
    m_db_file.setDatabaseName(path);

    if(!m_db_file.open()) m_error_status = apotek::errors::error_database_write;

    QStringList tables = m_db_file.tables();

    if(tables.contains("durability_products", Qt::CaseInsensitive)
            && tables.contains("products_saved", Qt::CaseInsensitive)) {
        qDebug() << "all tables set!";
    }

    // tables doesnt exists, create them

    //
    QSqlQuery q(m_db_file);
    if(!q.exec(QLatin1String("CREATE TABLE IF NOT EXISTS "
                             "durability_products(holdbarhet_id primary key, varenr integer, navn varchar, id varchar, ean varchar, "
                             "legemiddelform varchar, mengde integer, holdbarhet varchar, lokasjon varchar)")))
        m_error_status = apotek::errors::error_database_write;

    // table products_saved
    if(!q.exec(QLatin1String("CREATE TABLE IF NOT EXISTS "
                             "products_saved(varer_id primary key, varenr integer, navn varchar, id varchar, ean varchar,"
                             " legemiddelform varchar, mengde integer)")))
       m_error_status = apotek::errors::error_database_write;
}

void Database::init_db_memory()
{
    m_db_memory = QSqlDatabase::addDatabase("QSQLITE", "memory");
    m_db_memory.setDatabaseName(":memory:");

    //QString path = set_db_path();
    //m_db.setDatabaseName(path);

    if(!m_db_memory.open()) m_error_status = apotek::errors::error_database_write;

    QStringList tables = m_db_memory.tables();

    if(tables.contains("products", Qt::CaseInsensitive))
        qDebug() << "all tables set";

    QSqlQuery q(m_db_memory);
    // create tables if they doesnt exists.
    if(!q.exec(QLatin1String("CREATE TABLE IF NOT EXISTS products(varer_id primary key, varenr integer, navn varchar, id varchar, ean varchar, legemiddelform varchar, mengde integer)")))
        m_error_status = apotek::errors::error_database_write;

    // used to compare the fest_xml file with the database version
    if(!q.exec(QLatin1String("CREATE TABLE IF NOT EXISTS fest_info(fest_id primary key, hentetdato varchar)")))
        m_error_status = apotek::errors::error_database_write;
}

void Database::init_db()
{
    init_db_file();
    init_db_memory();
}

// gets all the varer from the xml file. Stores them in the data member m_products
// saves it to the database. It also fetches the custom made products from the sql file.
ProductsContainer Database::get_products()
{
    bool error{false};

    // get products from xml file
    m_products = get_from_xml();

    for(const auto& v : m_products) {
        error = save_product(std::move(v));
    }


    // get the saved custom products
    auto saved_products = get_newproducts();

    for(const auto& p : saved_products) {
        if(!check_if_product_exists(p.get_varenr())) {
            m_products.push_back(p);
            save_product(std::move(p));
        } else {
            error = true;
        }
    }

    if(error == true) m_error_status = apotek::errors::error_database_getproducts;

    return m_products;
}

// TODO: this is for future version
// this function does not work.
bool Database::compare_fest_hentetdato()
{
    // reads the xml file
    apotek::xml::FEST_Reader f;
    QString xml_date = f.get_fest_hentetdato();

    // read the database
    QString db_date = get_fest_hentetdato();

    // lets not compare two empty strings
    if(db_date.size() == 0) return false;
    if(xml_date.size() < 1) return false;

    if(QString::compare(db_date, xml_date, Qt::CaseInsensitive)) {
        return true;
    } else {
        return false;
    }
}


// get all the Products from the xml file
inline ProductsContainer Database::get_from_xml()
{
    bool error{false};

    auto xml_products = m_festreader.get_content();
    auto xml_products_vet = m_festreader_vet.get_content();

    for(const auto& p : xml_products_vet) {
        xml_products.push_back(p);
    }

    if(error) m_error_status = apotek::errors::error_xml_error;

    return xml_products;
}


// TODO: check if string is a dato
void Database::set_fest_hentetdato()
{
    QSqlQuery q(m_db_memory);
    if(!q.prepare("insert into fest_info (hentetdato) values (?)"))
        m_error_status = apotek::errors::error_database_write;

    auto hentetdato = m_festreader.get_fest_hentetdato();
    q.addBindValue(hentetdato);
    q.exec();
}

// fetches the version of the FEST xml file
QString Database::get_fest_hentetdato()
{
    QSqlQuery q(m_db_memory);
    q.prepare("select hentetdato from fest_info ORDER BY ROWID ASC LIMIT 1");

    if(!q.exec()) {
        m_error_status = apotek::errors::error_database_write;
    }

    q.first();

    QVariant hentetdato = q.value(0);
    QString fest_hentetdato = hentetdato.toString();

    // check for error
    if(fest_hentetdato.isEmpty())
        m_error_status = apotek::errors::error_database_write;

    return fest_hentetdato;
}

bool Database::check_if_product_exists(int varenr) const
{
    QSqlQuery q(m_db_memory);

    q.prepare("SELECT * FROM products where varenr = :varenr");

    q.bindValue(":varenr", varenr);
    q.exec();

    if(q.first()) {
        return true;
    } else {
        return false;
    }
}

bool Database::check_if_durability_exists(int varenr) const
{
    QSqlQuery q(m_db_file);

    q.prepare("SELECT * FROM durability_products where varenr = :varenr");
    q.bindValue(":varenr", varenr);
    q.exec();

    if(q.first()) {
        return true;
    } else {
        return false;
    }
}

ProductsContainer Database::navn_search_product(const QString& search_product) const
{
    ProductsContainer result;

    // find in m_holdbarhet
    // unsure if I want to implement this.
    /*
    for(auto it = std::find_if(std::cbegin(m_holdbarhet_varer), std::cend(m_holdbarhet_varer), Vare::Find_vare(search_vare)) ;
        it != std::end(m_holdbarhet_varer) ;
        it = std::find_if(++it, std::cend(m_holdbarhet_varer), Vare::Find_vare(search_vare)))
    {
        result.push_back(*it);
    }
    */

    // search in the m_vare
    for(auto it = std::find_if(std::cbegin(m_products), std::cend(m_products), Product::Find_vare(search_product));
        it != std::cend(m_products);
        it = std::find_if(++it, std::cend(m_products), Product::Find_vare(search_product)))
    {
        // check if its already saved in m_holdbarhet_varer
        if(!check_if_durability_exists(it->get_varenr()))
            result.push_back(*it);
    }
    return result;
}

ProductsContainer Database::ean_search_product(const QString& search_product) const
{
    ProductsContainer result;
    int s_varenr = search_product.toInt();

    // important to check holdbarhet first, because its most likely inside varer if the vare exists.

    if(check_if_durability_exists(s_varenr)) {
        auto it = std::find_if(std::cbegin(m_durability_products), std::cend(m_durability_products), Product::Find_ean(search_product));
        if(it!= std::cend(m_durability_products)) {
            result.push_back(*it);
            return result;
        }
    }


        auto it = std::find_if(std::cbegin(m_products), std::cend(m_products), Product::Find_ean(search_product));
        if(it != std::cend(m_products)) {
                result.push_back(*it);
    }
    return result;
}

ProductsContainer Database::datamatrix_search_product(const QString& search_product)
{
    //using namespace apotek::database;

    ProductsContainer result;
    FMD_decoder fmd(search_product);

    auto fmd_product = fmd.get_product();

    auto ean_results = ean_search_product(fmd_product.get_ean());

    if(ean_results.empty()) {
        // lets see if its in our ean_codes.xml file.
        FMD_xml_reader f;
        int result = f.find_code(fmd_product.get_ean());
        QString string_varenr{QString::number(result)};

        ean_results = varenr_search_product(string_varenr);
    }

    if(!ean_results.empty()) {
        if(!fmd_product.get_holdbarhet().isEmpty() && !fmd_product.get_holdbarhet().isEmpty())
            ean_results[0].set_holdbarhet(fmd_product.get_holdbarhet());
        save_durability(std::move(ean_results[0]));
        return ean_results;
    }

    std::vector<Product> varenr_results = varenr_search_product(QString::number(fmd_product.get_varenr()));

    if(!varenr_results.empty()) {
        return varenr_results;
    }
    return result;
}

ProductsContainer Database::varenr_search_product(const QString& search_vare) const
{
    ProductsContainer result;
    int s_varenr = search_vare.toInt();

    // important to check holdbarhet first, because its most likely inside varer if the vare exists.

    if(check_if_durability_exists(s_varenr)) {
        auto it = std::find_if(std::cbegin(m_durability_products), std::cend(m_durability_products), Product::Find_vare(s_varenr));
        if(it != std::cend(m_durability_products)) {
                result.push_back(*it);
            return result;
        }
    }


    if(check_if_product_exists(s_varenr)) {
        auto it = std::find_if(std::cbegin(m_products), std::cend(m_products), Product::Find_vare(s_varenr));
        if(it != std::cend(m_products)) {
            //if(!check_if_holdbarhet_exists(it->m_varenr))
                result.push_back(*it);
        }
    }
    return result;
}

// fethces the custom made products from the database.
ProductsContainer Database::get_newproducts() const
{
    QSqlQuery q(m_db_file);

    ProductsContainer result;

    if(q.exec("SELECT * FROM products_saved")) {
        while(q.next()) {
            Product p;
            p.set_varenr(q.value(1).toInt());
            p.set_navn(q.value(2).toString());
            p.set_ean(q.value(4).toString());
            p.set_legemiddelform(q.value(5).toString());
            p.set_mengde(q.value(6).toInt());

            result.push_back(p);
        }
    }

    return result;
}

QSqlError Database::update_durability()
{
     QSqlQuery q(m_db_file);

     if(q.exec("SELECT * FROM durability_products")) {
         // clear our vector
         m_durability_products.clear();
         // extract all values and store them in our vector
         while(q.next()) {
             Product p;
             p.set_varenr(q.value(1).toInt());
             p.set_navn(q.value(2).toString());
             p.set_ean(q.value(4).toString());
             p.set_legemiddelform(q.value(5).toString());
             p.set_mengde(q.value(6).toInt());
             p.set_holdbarhet(q.value(7).toString());
             p.set_lokasjon(q.value(8).toString());
             m_durability_products.push_back(p);
         }
     } else {
         return q.lastError();
     }
    return QSqlError();
}

QSqlError Database::save_durability(Product product)
{
    int s_varenr = product.get_varenr();

    if(check_if_durability_exists(s_varenr)) {
        update_durability_product(product);
        return QSqlError();
    }

    QSqlQuery q(m_db_file);

    // varenr integer, navn varchar, id varchar, ean varchar, holdbarhet varchar, lokasjon
    if(!q.prepare(QLatin1String("insert into durability_products(varenr, navn, id, ean, legemiddelform, "
                                "mengde, holdbarhet, lokasjon) "
                                "values (?, ?, ?, ?, ?, ?, ?, ?)"))) {
        m_error_status = errors::error_database_write;
        return q.lastError();
    }

    // extract what we need from the struct
    const QString navn{product.get_navn()};
    const QString id{product.get_id()};
    const QString ean{product.get_ean()};
    const QString legemiddelform{product.get_legemiddelform()};
    const int mengde{product.get_mengde()};
    const QString holdbarhet{product.get_holdbarhet()};
    const QString lokasjon{product.get_lokasjon()};
    const int varenr{product.get_varenr()};

    q.addBindValue(varenr);
    q.addBindValue(navn);
    q.addBindValue(id);
    q.addBindValue(ean);
    q.addBindValue(legemiddelform);
    q.addBindValue(mengde);
    q.addBindValue(holdbarhet);
    q.addBindValue(lokasjon);

    if(!q.exec()) {
        m_error_status = errors::error_database_write;
        return q.lastError();
    }

    return q.lastError();
}

QSqlError Database::remove_durability(const Product &product)
{

    QSqlQuery q(m_db_file);
    int s_varenr = product.get_varenr();

    if(check_if_durability_exists(s_varenr)) {
        int varenr = product.get_varenr();
        q.prepare("DELETE FROM durability_products WHERE varenr = ?");
        q.addBindValue(varenr);
        q.exec();
        return QSqlError();
    } else {
        return q.lastError();
    }
}


bool Database::check_if_newproduct_exists(const Product& product) const
{
    int product_varenr = product.get_varenr();
    QString product_ean = product.get_ean();

    auto result_varenr = varenr_search_product(QString::number(product_varenr));

    // the product already exists
    if(!result_varenr.empty()) return true;

    auto result_ean = ean_search_product(product_ean);

    if(!result_ean.empty()) return true;

    // all checks passed, the product doesn't exist
    return false;
}


// this saves the product to products_saved table in the database file.
// the function is called from add_newproduct - it checks if the product is already
// saved in the memory. You should not call this function alone.
bool Database::save_newproduct(const Product& product)
{
    if(!check_if_newproduct_exists(product)) {
        QSqlQuery q(m_db_file);

        if(!q.prepare(QLatin1String("insert into products_saved(varenr, navn, ean, "
                                    "legemiddelform, mengde) values (?, ?, ?, ?, ?)"))) {
            m_error_status = errors::error_database_write;
            return false;
        }

        const int varenr{product.get_varenr()};
        const QString navn{product.get_navn()};
        const QString ean{product.get_ean()};
        const int mengde{product.get_mengde()};
        const QString legemiddelform{product.get_legemiddelform()};

        q.addBindValue(varenr);
        q.addBindValue(navn);
        q.addBindValue(ean);
        q.addBindValue(legemiddelform);
        q.addBindValue(mengde);

        if(!q.exec()) {
            m_error_status = errors::error_database_write;
            return false;
        }

        m_products.push_back(product);
        return true;
    } else {
        return false;
    }
}

} // namespace
} // namespace
