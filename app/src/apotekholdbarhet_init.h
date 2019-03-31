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

#ifndef APOTEKH_INIT_H
#define APOTEKH_INIT_H

#include "../ui/apotekholdbarhet.h"

namespace apotek{
namespace apotekholdbarhet {

// non-member class for ApotekHoldbarhet.
// initiate the ui with size, title etc.
void apotekholdbarhet_init(ApotekHoldbarhet& ah)
{
    ah.setFixedSize(1260, 768);
    ah.set_title();
    ah.fetch_products();
    ah.set_label_colors();
}

}
}

#endif // APOTEKHOLDBARHET_INIT_H
