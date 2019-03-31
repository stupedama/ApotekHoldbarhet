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

#ifndef APOTEKH_CHECK_NUMBERS_H
#define APOTEKH_CHECK_NUMBERS_H

#include <QString>

namespace apotek {
namespace apotekholdbarhet{

// checks if the string only contains numbers or not.
bool check_numbers(const QString& s);

} // namespace
} // namespace

#endif // CHECK_NUMBERS_H
