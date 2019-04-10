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

#include "calculate_months.h"

namespace apotek{
namespace apotekholdbarhet{

int calculate_months(const QString& holdbarhet)
{
    using namespace date;
    sys_days today = year_month_day{floor<days>(std::chrono::system_clock::now())};
    QDate holdbarhet_date = QDate::fromString(holdbarhet, apotek::constants::date_format);

    sys_days holdbarhet_sysdays = sys_days{days{holdbarhet_date.toJulianDay()} -
            (sys_days{1970_y/jan/1} - sys_days{year{-4713}/nov/24})};

    return round<months>(holdbarhet_sysdays-today).count();
}

}
}
