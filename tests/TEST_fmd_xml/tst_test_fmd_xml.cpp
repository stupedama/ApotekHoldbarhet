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

#include <QtTest>
#include <QString>
#include "fmd/fmd_xml_reader.h"
#include "fmd/fmd_decoder.h"
#include "fmd/fmd_code.h"


class TEST_fmd_xml : public QObject
{
    Q_OBJECT

public:
    TEST_fmd_xml();
    ~TEST_fmd_xml();

private slots:
    void find_betnovat();

};

TEST_fmd_xml::TEST_fmd_xml()
{

}

TEST_fmd_xml::~TEST_fmd_xml()
{

}

void TEST_fmd_xml::find_betnovat()
{
    using namespace apotek::database;

    // betnovat data matrix
    QString betnovat_matrix{"010541506231373221985741683713{GS}1721033110X69574"};

    // decode the matrix first
    FMD_decoder decode(betnovat_matrix);
    auto r = decode.get_product();

    // read the xml file and find the varenr.
    FMD_xml_reader f;
    auto result = f.find_code(r.get_ean());

    QVERIFY(result == 8524);
}


QTEST_APPLESS_MAIN(TEST_fmd_xml)

#include "tst_test_fmd_xml.moc"
