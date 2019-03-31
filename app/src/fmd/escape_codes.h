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

#ifndef APOTEKH_ESCAPE_CODES_H
#define APOTEKH_ESCAPE_CODES_H

#include <QString>

namespace apotek {
namespace constants{

const QString escape_code_lot{"10"};
const QString escape_code_durability{"17"};
const QString escape_code_sn{"21"};
const QString escape_code_ean{"01"};

} // namespace
} // namespace

#endif // APOTEKH_ESCAPE_CODES_H
