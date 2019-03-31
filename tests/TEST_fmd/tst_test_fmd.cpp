#include <QtTest>
#include <QString>
#include "product.h"
#include "fmd_decoder.h"

class TEST_fmd : public QObject
{
    Q_OBJECT

public:
    TEST_fmd();
    ~TEST_fmd();

private slots:
    void test_varenr() const;
    void test_ean() const;
    void test_durability() const;

private:
    QString m_matrix_case1;
    QString m_matrix_case2;
    QString m_matrix_case3;

};

TEST_fmd::TEST_fmd()
    // Etoricoxib Orion Tab 120 mg - ean: 6432100029294 - exp date: 04/2020.
    : m_matrix_case1{"010643210002929417200400101705005014{GS}21X7232JDD4D77"},
      m_matrix_case2{"010541506231373221985741683713{GS}1721033110X69574"},
      m_matrix_case3{"01050546261508281720090010TN8T{GS}215BZFD0CT5C"}
{
}

TEST_fmd::~TEST_fmd()
{

}

void TEST_fmd::test_varenr() const
{
    using namespace apotek::database;

    FMD_decoder t1(m_matrix_case1);
    auto p1 = t1.get_product();
    QCOMPARE(p1.get_varenr(), 2929);

    FMD_decoder t2(m_matrix_case2);
    auto p2 = t2.get_product();
    QCOMPARE(p2.get_varenr(), 231373);
}

void TEST_fmd::test_ean() const
{
    using namespace apotek::database;

    FMD_decoder t1(m_matrix_case1);
    auto p1 = t1.get_product();

    QCOMPARE(p1.get_ean(), "6432100029294");

    FMD_decoder t2(m_matrix_case2);
    auto p2 = t2.get_product();

    QCOMPARE(p2.get_ean(), "5415062313732");
}

void TEST_fmd::test_durability() const
{
    using namespace apotek::database;

    FMD_decoder t1(m_matrix_case1);
    auto p1 = t1.get_product();

    QCOMPARE(p1.get_holdbarhet(), "200428");
}

QTEST_APPLESS_MAIN(TEST_fmd)

#include "tst_test_fmd.moc"
