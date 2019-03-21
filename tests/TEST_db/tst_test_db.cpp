#include <QtTest>
#include <QDebug>
#include <database.h>
#include <product.h>
#include <fest_xml/fest_reader.h>

class TEST_db : public QObject
{
    Q_OBJECT
public:
    TEST_db();
private slots:
    void fetch_xml();
    void save_all_to_database();
    void get_products();
    void save_durability();
    void get_durability();
    void search_product();
    void search_name();
    //void search_ean();
    void search_durability();
    void add_duplicate();

private:
    apotek::xml::FEST_Reader f;
    apotek::database::Database d;
    std::vector<apotek::database::Product> m_products;
    std::vector<apotek::database::Product> m_durability;
};

TEST_db::TEST_db()
{
    qDebug() << "Hello. Tests will take several minutes to complete.";
}

void TEST_db::fetch_xml()
{
    m_products = f.get_content();

    qDebug() << "size of m_varer: " << m_products.size();

    QVERIFY(!m_products.empty());
}

void TEST_db::save_all_to_database()
{
    QBENCHMARK {
        for(const auto& p : m_products)
            d.add_product_from_xml(p);
    }
}

void TEST_db::save_durability()
{
    // a imaginary vare
    apotek::database::Product v;
    v.set_id("10212");
    v.set_varenr(12345);
    v.set_ean("130981309812308123");
    v.set_navn("Happy pills 100 mg");
    v.set_legemiddelform("pills");
    v.set_holdbarhet("02 02 2018");
    v.set_lokasjon("RC0202");

    QSqlError err = d.save_durability(v);
    QVERIFY(!err.NoError);
}


void TEST_db::get_products()
{
    auto products = d.get_products();

    // there should be atleast be 100 varer in the database
    QVERIFY(products.size() > 100);
}


void TEST_db::get_durability()
{
    std::vector<apotek::database::Product> durability = d.get_durability();

    // there should be atleast one vare in holdbarhet from TEST_db::save_holdbarhet()
    QVERIFY(!durability.empty());
}

void TEST_db::search_product()
{
    std::vector<apotek::database::Product> result;

    QBENCHMARK {
        result = d.search_product("tab");
    }

    qDebug() << "size of search result: " << result.size();
}

void TEST_db::search_durability()
{
    // we saved this one later. the search algorithm searches the holdbarhet database first.
    // so this should return true

    auto result = d.search_product("12345");
    QVERIFY(!result.empty());
}


void TEST_db::search_name()
{
    auto result = d.search_product("tab");
    QVERIFY(!result.empty());
}

void TEST_db::add_duplicate()
{
    bool error{false};

    apotek::database::Product p;
    p.set_id("1022");
    p.set_varenr(123451);
    p.set_ean("13092381309812308123");
    p.set_navn("Happy pills 200 mg");
    p.set_legemiddelform("pills");
    //p.m_holdbarhet = "02 03 2018";
    //p.m_lokasjon = "RC0203";

    // try to add the same twice
    error = d.add_product_from_xml(p);
    error = d.add_product_from_xml(p);

    QVERIFY(error == true);

}

QTEST_APPLESS_MAIN(TEST_db)

#include "tst_test_db.moc"
