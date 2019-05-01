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

#ifndef APOTEKH_FMD_XML_READER_H
#define APOTEKH_FMD_XML_READER_H

#include <QString>
#include <string>
#include <vector>
#include <tinyxml2.h>
#include "fmd_code.h"

#include <QDebug>

namespace apotek {
namespace database {


/**
 * @brief The FMD_xml_reader class
 * reads the fmd/ean_codes.xml file and stores them in a vector<FMD_code>.
 */
class FMD_xml_reader
{
public:
    explicit FMD_xml_reader();
    ~FMD_xml_reader() = default;
    std::vector<FMD_code> get_codes() { return m_ean_codes; }
    int find_code(const QString& ean);
private:
    inline void read_file();
    std::vector<FMD_code> m_ean_codes;
    std::string m_filename;
};

inline void FMD_xml_reader::read_file()
{
    using namespace tinyxml2;

    XMLDocument doc;

    XMLError read_file_error = doc.LoadFile(m_filename.c_str());
    if(read_file_error != XML_SUCCESS) return;


    XMLNode* node = doc.FirstChildElement("ean_codes")->FirstChildElement("drug");
    if(node == nullptr) return;

    while(node != nullptr) {

        // -1 means failed code.
        int varenr{-1};
        QString ean;

        if(node->FirstChildElement("stockno")) {
            varenr = node->FirstChildElement("stockno")->IntText();
        }

        if(node->FirstChildElement("ean")) {
            ean = node->FirstChildElement("ean")->GetText();
        }

        if(varenr != -1) {
            FMD_code code{varenr, std::move(ean)};
            m_ean_codes.push_back(code);
        }

        node = node->NextSiblingElement();
    }
}

} // namespace
} // namespace

#endif // FMD_XML_READER_H
