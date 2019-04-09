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

#ifndef APOTEKH_FMD_CODES_H
#define APOTEKH_FMD_CODES_H

#include <QString>

/**
 * @brief The FMD_code struct
 * Struct that stores the datatypes from the fmd/ean_codes.xml
 * it also handles searching by struct find_code.
 */
struct FMD_code
{
    explicit FMD_code();
    explicit FMD_code(int varenr, const QString& ean)
        : m_varenr{varenr}, m_ean{ean} {}
    ~FMD_code() = default;
    int m_varenr;
    QString m_ean;

    struct find_code;
};


// used in std::find_if to search the vector finding the ean.
// usally from a data matrix code.
struct FMD_code::find_code{
    find_code(const QString& ean) : m_ean{ean} {}

    bool operator() (const FMD_code& rhs) {
        int r = QString::compare(m_ean, rhs.m_ean);

        if(r == 0) {
            return true;
        } else {
            return false;
        }
    }

    QString m_ean;
};

#endif // FMD_CODES_H
