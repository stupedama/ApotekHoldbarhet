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
