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
#include "fmd_xml_reader.h"

namespace apotek {
namespace database {


FMD_xml_reader::FMD_xml_reader()
    : m_filename{"./fmd/ean_codes.xml"}
{
    read_file();
}

// search for a varenr and find the scan code
int FMD_xml_reader::find_code(const QString& ean)
{
    // return -1 if its not found.
    int varenr{-1};

    // lets try to find it with 0 first
    // most of the codes in the xml file begins with 0.
    auto it2 = std::find_if(std::begin(m_ean_codes), std::end(m_ean_codes), FMD_code::find_code("0" + ean));

    if(it2 != std::end(m_ean_codes)) {
        return it2->m_varenr;
    }

    auto it = std::find_if(std::begin(m_ean_codes), std::end(m_ean_codes), FMD_code::find_code(ean));

    if(it != std::end(m_ean_codes)) {
        return it->m_varenr;
    }

    return varenr;
}

} // namespace
} // namespace
