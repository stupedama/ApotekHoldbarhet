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

#ifndef ERROR_MESSAGES_H
#define ERROR_MESSAGES_H

#include <QString>

namespace apotek {
namespace errors {

// TODO: add namespaces to the different errors?
// example database errors inside namespace database (?)

// vector errors
const QString error_vector_size{"Feil. Det er lagret alt for mange varer - vennlig avslutt programmet."};
const QString error_vector_size_soon{"Feil. Du har lagret for mange varer. Vær vennlig å begynn å slett noen fra listen."};
const QString error_no_products{"Det er ingen varer i databasen. Last ned FEST og legg det i mappen fest/ og start programmet på nytt."};

// database errors
const QString error_database_init{"Database feil: Kunne ikke starte databasen."};
const QString error_database_write{"Database feil: Kunne ikke skrive til databasen."};
const QString error_database_getproducts{"Database feil: Kunne ikke finne noen varer."};

// xml errors
const QString error_xml_error{"Kunne ikke finne noen varer fra FEST filen."};

// ui errors
const QString error_ui_no_product{"Kunne ikke finne en vare. Vennligst prøv igjen."};
const QString error_ui_no_result{"Vennligst fyll inn varenr, navn på vare eller ta strekkode på varen."};

// Add vare ui
const QString error_ui_addvare_onlynumbers{"Dette feltet kan kun inneholde tall."};
const QString error_ui_addvare_empty{"Du har tomt/tomme felt."};
const QString error_ui_no_mengde{"Du må skrive inn et tall."};
const QString error_ui_addvare_onlychars{"Dette feltet kan kun inneholde bokstaver."};
const QString error_ui_mengde_maybe_ean{"Det er for mange tall i varenummeret. Det er kanskje en strekkode?"};
const QString error_ui_product_invalid{"Varen kunne ikke lagres. Mest sannsynligvis finnes varenr eller ean allerede."};

// FMD
const QString error_fmd_wrong_format{"2d koden er i feil format."};
const QString error_fmd_no_code{"Du må scanne 2d koden."};

} // namespace
} // namespace

#endif // ERROR_MESSAGES_H
