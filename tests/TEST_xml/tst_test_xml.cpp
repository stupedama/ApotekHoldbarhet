#include <QTest>
#include <QDebug>
#include <vector>
#include <product.h>
#include <fest_xml/fest_reader.h>

class TEST_xml : public QObject
{
    Q_OBJECT
private slots:
    void benchmark_xml_fetch();
    void check_xml_size();

private:
    std::vector<apotek::database::Product> m_products;
    apotek::xml::FEST_Reader f;
};



void TEST_xml::benchmark_xml_fetch()
{
    QBENCHMARK {
        m_products = f.get_content();
    }
}

void TEST_xml::check_xml_size()
{
    QVERIFY(m_products.size() > 800);
}

QTEST_APPLESS_MAIN(TEST_xml)

#include "tst_test_xml.moc"
