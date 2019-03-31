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

#include "fmd_decoder.h"

namespace apotek{
namespace database{

FMD_decoder::FMD_decoder(const QString& matrix_code)
    : m_matrix_code{matrix_code}, m_error_code{"none"}
{
}

Product FMD_decoder::get_product()
{
    Product p;
    p.set_varenr(find_varenr());
    p.set_ean(find_ean());
    p.set_holdbarhet(find_durability());
    return p;
}

// checks if a datamatrix is valid, all should start with escape_code_ean.
// and obvisouly not empty.
bool FMD_decoder::check_valid_string()
{
    using namespace apotek::constants;
    using namespace apotek::errors;

    if(m_matrix_code.isEmpty()) {
        m_error_code = apotek::errors::error_fmd_no_code;
        return false;
    }

    // all the matrix codes starts with the escape_code_ean
    if(!m_matrix_code.startsWith(escape_code_ean)) {
        m_error_code = error_fmd_wrong_format;
        return false;
    } else {
        return true;
    }
}

// find the scancode (ean/PC) of the data matrix code
// the manual says it starts with escape_code_ean
QString FMD_decoder::find_escape_code(const QString& escape_code)
{
    using namespace apotek::constants;

    QString result;
    QString search_string = m_matrix_code;
    QString rx_string;

    if(check_valid_string()) {

        // remove the first two characters if we are looking for ean/pc.
        if(escape_code == escape_code_ean) {
            search_string.remove(0, 2);
            rx_string = {"\\S+(\\B" + escape_code + ")"};
        } else {
            rx_string = {"(\\B" + escape_code + ")\\S+"};
        }

        QRegExp rx{rx_string};

        if(rx.indexIn(search_string) != regexp_error) {
            QStringList lst = rx.capturedTexts();
            result = lst.at(0);

            if(!result.isEmpty()) {
                result.remove(escape_code);
            }
        }
    }

    return result;
}

// the durability is always 6 digits.
// ther are two formats, one with durability with year/month and year/month/day.
QString FMD_decoder::find_durability()
{
    using namespace apotek::constants;

    QString result = find_escape_code(escape_code_durability);
    result.remove(fmd_durability_size, result.size()-fmd_durability_size);

    // if the durability ends with 00, its durability is only month and year,
    // lets replace them with 28 as all months have 28 days.
    // TODO: use date library to find the last day of the month.
    if(result.contains("00")) {
        result.replace(4, 2, "28");
    }

    return result;
}

// find the ean/PC from the data matrix.
QString FMD_decoder::find_ean() const
{
    QString result = m_matrix_code;

    using namespace apotek::constants;
    // the matrix code always with the apotek::constants::escape_code_ean
    if(m_matrix_code.startsWith(escape_code_ean)) {
        if(m_matrix_code.startsWith(escape_code_ean + "0")) {
            result.remove(0, 3);
            result.remove(13, result.size());
        }
    }

    return result;
}

// find the varenr
int FMD_decoder::find_varenr() const
{
    int varenr{0};
    QString ean = find_ean();

    // the varenr is sometimes hidden in the ean after 6 numbers
    // and the last digit is not included in the stockno.
    ean.remove(0, 6);
    ean.resize(ean.size()-1);

    varenr = ean.toInt();
    return varenr;
}

/*
QString FMD_decoder::find_breakpoint_code(const QString& start_escape_code, const QString& end_escape_code)
{
    QString result;

    if(check_valid_string()) {
        QString regex_string{"(\\B" + start_escape_code + ")(\\S+)(\\B" + end_escape_code + ")"};
        QRegExp rx(regex_string);

        if(rx.indexIn(m_matrix_code) != apotek::constants::regexp_error) {
            QStringList lst = rx.capturedTexts();
            result = lst.at(0);

            if(!result.isEmpty()) {
                result.remove(start_escape_code);
                result.remove(end_escape_code);
            }
        }
    }
    return result;
}
*/

} // namespace
} // namespace
