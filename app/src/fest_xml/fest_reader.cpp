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

#include "fest_reader.h"

namespace apotek {
namespace xml {

QString FEST_Reader::get_fest_hentetdato()
{
    using namespace tinyxml2;

    XMLDocument doc;

    tinyxml2::XMLError read_file_error = doc.LoadFile(m_filename.c_str());
    if(read_file_error != tinyxml2::XML_SUCCESS) return "error";

    XMLNode* node = doc.FirstChildElement("FEST");
    if(node == nullptr) return "error";

    std::string dato = node->FirstChildElement("HentetDato")->GetText();

    return QString::fromStdString(dato);
}

std::vector<apotek::database::Product> FEST_Reader::get_content()
{
    m_varer.clear();
    read_file();

    return m_varer;
}

} // namespace
} // namespace
