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

#ifndef DATABASE_INIT_H
#define DATABASE_INIT_H

#include "database.h"

namespace apotek{
namespace database{

// non-member for Database.
// inits the two databases and gets the date from the xml file.
void database_init(Database& db)
{
    db.init_db_file();
    db.init_db_memory();
    db.set_fest_hentetdato();
}

} // namespace
} // namespace

#endif // DATABASE_INIT_H
