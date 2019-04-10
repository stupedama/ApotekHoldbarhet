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

#ifndef APOTEKH_VERSION_H
#define APOTEKH_VERSION_H

#include <QString>

namespace apotek {
namespace apotekholdbarhet {

constexpr int MAJOR{0};
constexpr int MINOR{6};
constexpr int PATCH{0};

// version of the program
const QString apotek_program_version = "v"
        + QString::number(MAJOR)
        + "." + QString::number(MINOR)
        + "." + QString::number(PATCH);


} // namespace
} // namespace

#endif // APOTEKH_VERSION_H
