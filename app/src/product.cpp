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

#include "product.h"

namespace apotek {
namespace database {

Product::Product(const QString& id, const QString& name,
                 int varenr, const QString& ean, const QString& legemiddelform, int mengde)
    :    m_id{id}, m_navn{name}, m_varenr{varenr}, m_ean{ean}, m_legemiddelform{legemiddelform}, m_mengde{mengde}
{
}

// checks if the Product is a valid product
// m_varenr and m_navn cannot be empty
// it also checks if varenr, ean and mengde is numbers.
bool Product::sanity_check() const
{
    using apotek::apotekholdbarhet::check_numbers;

    bool sanity{false};

    const QString string_mengde = QString::number(m_mengde);
    const QString string_varenr = QString::number(m_varenr);

    if(check_numbers(string_varenr)) {
        sanity = true;
    } else {
        sanity = false;
    }

    if(check_numbers(string_mengde)) {
        sanity = true;
    } else {
        sanity = false;
    }

    if(check_numbers(m_ean)) {
        sanity = true;
    } else {
        sanity = false;
    }

    if(string_varenr.isEmpty() || m_navn.isEmpty()) sanity = false;

    return sanity;
}

} // namespace
} // namespace
