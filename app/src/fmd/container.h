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

#ifndef FMD_CONTAINER_H
#define FMD_CONTAINER_H

#include <vector>

namespace apotek{
namespace database{

// forward declaration
class FMD_code;

// the container for the FMD_code class used for the matrix codes
using FMDContainer = std::vector<FMD_code>;

} // namespace
} // namespace

#endif // FMD_CONTAINER_H
