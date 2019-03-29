#include <QtTest>

// add necessary includes here

class TEST_fmd : public QObject
{
    Q_OBJECT

public:
    TEST_fmd();
    ~TEST_fmd();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void test_case1();

};

TEST_fmd::TEST_fmd()
{

}

TEST_fmd::~TEST_fmd()
{

}

void TEST_fmd::initTestCase()
{

}

void TEST_fmd::cleanupTestCase()
{

}

void TEST_fmd::test_case1()
{

}

QTEST_APPLESS_MAIN(TEST_fmd)

#include "tst_test_fmd.moc"
