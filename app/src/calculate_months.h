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

#ifndef APOTEKH_CALCULATE_MONTHS_H
#define APOTEKH_CALCULATE_MONTHS_H

#include "constants.h"
#include <QDate>
#include <QString>
#include <date.h>

namespace apotek{
namespace apotekholdbarhet{

int calculate_months(const QString& holdbarhet);

} // namespace
} // namespace

#endif // CALCULATE_MONTHS_H
