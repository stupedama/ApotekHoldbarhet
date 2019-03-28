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
