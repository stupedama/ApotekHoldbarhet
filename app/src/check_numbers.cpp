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

#include <QString>
#include <QRegExp>

namespace apotek {
namespace apotekholdbarhet{

// checks if the QString is a number or not
// returns true if string is only numbers, otherwise returns false.
bool check_numbers(const QString& s)
{
    const QRegExp check_string("\\d*");

    if(check_string.exactMatch(s)) {
        return true;
    } else {
        return false;
    }
}

} // namespace
} // namespace
