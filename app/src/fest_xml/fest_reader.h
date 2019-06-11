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

#ifndef FEST_READER_H
#define FEST_READER_H

#include "../product.h"
#include "../container.h"
#include "../constants.h"
#include <string>
#include <QString>
#include <tinyxml2.h>

// TODO: Need to read <KatHandelsvare> too get more varer.

namespace apotek {
namespace xml {
/**
 * @brief The FEST_Reader class
 * Reads the fest_xml file from Legemiddelverket by using the tinyxml2 library.
 * The xml file uses Norwegian nomenclature on their sections. The stuff we are interesting in:
 *
 * navn = <NavnFormStyrke>
 * id = <Id>
 * varenr = <Varenr>
 * ean = <Ean>
 * legemiddelform = <LegemiddelformKort>
 * mengde = <Pakningsinfo><Mengde>
 *
 */
class FEST_Reader
{
public:
    explicit FEST_Reader() : m_filename{"./fest/fest251_inst.xml"}
    {
        m_varer.reserve(apotek::constants::reserve_product_size);
    }
    explicit FEST_Reader(std::string f) : m_filename{std::move(f)}
    {
        m_varer.reserve(apotek::constants::reserve_product_size);
    }
    ~FEST_Reader() = default;
    std::string get_filename() { return m_filename; }
    ProductsContainer get_content();
    QString get_fest_hentetdato();
private:
    ProductsContainer m_varer;
    std::string m_filename;
    inline void read_file();
};


// Reads the whole FEST xml file - stores them in a std::vector<Product>
inline void FEST_Reader::read_file()
{
    using namespace tinyxml2;
    using namespace apotek::database;
    XMLDocument doc;

    XMLError read_file_error = doc.LoadFile(m_filename.c_str());
    if(read_file_error != XML_SUCCESS) return;

    XMLNode* node = doc.FirstChildElement("FEST")->FirstChildElement("KatLegemiddelpakning")->FirstChildElement("OppfLegemiddelpakning");
    if(node == nullptr) return;

    // QString -1 or int -1 means error.
    while(node != nullptr) {
        QString id = node->FirstChildElement("Id")->GetText();
        QString navn = node->FirstChildElement("Legemiddelpakning")->FirstChildElement("NavnFormStyrke")->GetText();
        int varenr = node->FirstChildElement("Legemiddelpakning")->FirstChildElement("Varenr")->IntText();

        QString ean{"-1"};
        if(node->FirstChildElement("Legemiddelpakning")->FirstChildElement("Ean")) {
            ean = node->FirstChildElement("Legemiddelpakning")->FirstChildElement("Ean")->GetText();
        }

        QString legemiddelform{"-1"};
        if(node->FirstChildElement("Legemiddelpakning")->FirstChildElement("LegemiddelformKort")->FindAttribute("DN")) {
            legemiddelform = node->FirstChildElement("Legemiddelpakning")->FirstChildElement("LegemiddelformKort")->FindAttribute("DN")->Value();
        }

        int mengde{-1};
        if(node->FirstChildElement("Legemiddelpakning")->FirstChildElement("Pakningsinfo")->FirstChildElement("Mengde")) {
            mengde = node->FirstChildElement("Legemiddelpakning")->FirstChildElement("Pakningsinfo")->FirstChildElement("Mengde")->IntText();
        }

        auto v = Product::make_product(id, navn, varenr, ean, legemiddelform, mengde);
        m_varer.push_back(v);
        node = node->NextSiblingElement();
    }
}

} // namespace
} // namespace
#endif // FEST_READER_H
