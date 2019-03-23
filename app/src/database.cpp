#include "database.h"
#include "database_init.h"

#ifdef _WIN32
    #include <ShlObj.h>
#endif

namespace apotek {
namespace database {

Database::Database()
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

// Gets all the varer from the sql database. if the database is empty or less than 200 varer, read the
// fest xml file and store them into the database and m_vare vector.
// returns the vector with all the varer.
std::vector<Product> Database::get_products()
{
    bool error{false};

    QSqlError err = update_products();

    if(m_products.size() < 200) {

        m_products = get_from_xml();

        for(const auto& v : m_products) {
            error = add_product_from_xml(v);
        }
        return m_products;
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

// fetches all the varer from the xml file to a std::vector<Vare> and
// saves them in the database, and when finished returns a std::vector<Vare> with all
// the varer.
inline std::vector<Product> Database::get_from_xml()
{
    bool error{false};

    auto xml_products = m_festreader.get_content();
    std::vector<Product> products;

    // save the xml varer into the database
    for(const auto& v : xml_products) {
        error = add_product_from_xml(v);
        products.push_back(v);
    }

    // TODO: make a warning in the ui, or handle the error.
    if(error) qDebug() << "database error";

    return products;
}

// TODO: check if string is a dato
void Database::add_fest_hentetdato()
{
    QSqlQuery q(m_db_memory);
    if(!q.prepare("insert into fest_info (hentetdato) values (?)"))
        m_error_status = apotek::errors::error_database_write;

    QString hentetdato = m_festreader.get_fest_hentetdato();
    q.addBindValue(hentetdato);
    q.exec();
}

// this is for future versions - not in use.
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

std::vector<Product> Database::navn_search_product(const QString& search_product) const
{
    std::vector<Product> result;

    // find in m_holdbarhet
    // unsure if I want to implement this.
    /*
    for(auto it = std::find_if(std::begin(m_holdbarhet_varer), std::end(m_holdbarhet_varer), Vare::Find_vare(search_vare)) ;
        it != std::end(m_holdbarhet_varer) ;
        it = std::find_if(++it, std::end(m_holdbarhet_varer), Vare::Find_vare(search_vare)))
    {
        result.push_back(*it);
    }
    */

    // search in the m_vare
    for(auto it = std::find_if(std::begin(m_products), std::end(m_products), Product::Find_vare(search_product));
        it != std::end(m_products);
        it = std::find_if(++it, std::end(m_products), Product::Find_vare(search_product)))
    {
        // check if its already saved in m_holdbarhet_varer
        if(!check_if_durability_exists(it->get_varenr()))
            result.push_back(*it);
    }
    return result;
}

std::vector<Product> Database::ean_search_product(const QString& search_product) const
{
    std::vector<Product> result;
    int s_varenr = search_product.toInt();

    // important to check holdbarhet first, because its most likely inside varer if the vare exists.

    if(check_if_durability_exists(s_varenr)) {
        auto it = std::find_if(std::begin(m_durability_products), std::end(m_durability_products), Product::Find_ean(search_product));
        if(it!= std::end(m_durability_products)) {
            result.push_back(*it);
            return result;
        }
    }


        auto it = std::find_if(std::begin(m_products), std::end(m_products), Product::Find_ean(search_product));
        if(it != std::end(m_products)) {
                result.push_back(*it);
    }
    return result;
}

std::vector<Product> Database::varenr_search_product(const QString& search_vare) const
{
    std::vector<Product> result;
    int s_varenr = search_vare.toInt();

    // important to check holdbarhet first, because its most likely inside varer if the vare exists.

    if(check_if_durability_exists(s_varenr)) {
        auto it = std::find_if(std::begin(m_durability_products), std::end(m_durability_products), Product::Find_vare(s_varenr));
        if(it != std::end(m_durability_products)) {
                result.push_back(*it);
            return result;
        }
    }


    if(check_if_product_exists(s_varenr)) {
        auto it = std::find_if(std::begin(m_products), std::end(m_products), Product::Find_vare(s_varenr));
        if(it != std::end(m_products)) {
            //if(!check_if_holdbarhet_exists(it->m_varenr))
                result.push_back(*it);
        }
    }
    return result;
}

QSqlError Database::update_products()
{
    QSqlQuery q(m_db_memory);

    if(q.exec("SELECT * FROM products")) {
        // clear our vector
        m_products.clear();
        // extract all values and store them in our vector
        while(q.next()) {
            Product p;
            p.set_varenr(q.value(1).toInt());
            p.set_navn(q.value(2).toString());
            p.set_ean(q.value(4).toString());
            p.set_legemiddelform(q.value(5).toString());
            p.set_mengde(q.value(6).toInt());

            m_products.push_back(p);
        }
    } else {
        return q.lastError();
    }
   return QSqlError();
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

QSqlError Database::save_durability(const Product& product)
{
    int s_varenr = product.get_varenr();

    if(check_if_durability_exists(s_varenr)) {
        update_durability_product(product);
        return QSqlError();
    }

    QSqlQuery q(m_db_file);

    // varenr integer, navn varchar, id varchar, ean varchar, holdbarhet varchar, lokasjon
    if(!q.prepare(QLatin1String("insert into durability_products(varenr, navn, id, ean, legemiddelform, mengde, holdbarhet, lokasjon) "
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

} // namespace
} // namespace
