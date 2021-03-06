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


#ifndef APOTEKH_CONTAINER_H
#define APOTEKH_CONTAINER_H

#include <vector>
#include <memory>

namespace apotek {

// forward declaration
namespace database{
class Product;
}

// container used for all the products and durability products in the ApotekHoldbarhet application.
using ProductsContainer = std::vector<std::shared_ptr<database::Product>>;

} // namespace

#endif // APOTEKH_CONTAINER_H
