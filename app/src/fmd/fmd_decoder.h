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

#ifndef APOTEKH_FMD_2DCODE_H
#define APOTEKH_FMD_2DCODE_H

#include <QString>
#include <QStringRef>
#include <QDate>
#include <QRegExp>
#include <QStringList>
#include <date.h>
#include "escape_codes.h"
#include "../product.h"
#include "../constants.h"
#include "../error_messages.h"

namespace apotek {
namespace database {

/**
 * @brief The FMD_2dcode class
 * Converts a data matrix code to ean, durability date and stocknr(varenr).
 */
class FMD_decoder
{
public:
    FMD_decoder(const QString &matrix_code);
    ~FMD_decoder() = default;
    Product get_product();
    QString get_error_code() const { return m_error_code; }
private:
    QString find_ean() const;
    QString find_durability();
    bool check_valid_string();
    QDate make_date(const QString& durability) const;
    QString find_escape_code(const QString& escape_code);
    QString remove_from_string(QString string, int fmd_size) const;
    int find_varenr() const;
    QString m_matrix_code;
    QString m_error_code;
};

} // namespace
} // namespace

#endif // FMD_2DCODE_H
