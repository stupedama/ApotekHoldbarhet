#include "fmd_decoder.h"

namespace apotek{
namespace database{

/*
const QString escape_code_lot{"10"};
const QString escape_code_durability{"17"};
const QString escape_code_sn{"21"};

// always start with 01.
const QString escape_code_ean{"01"};
*/

FMD_decoder::FMD_decoder(QString&& matrix_code)
    : m_matrix_code{matrix_code}, m_error_code{"none"}
{
}

Product FMD_decoder::get_product()
{
    Product p;
    p.set_varenr(find_stockno());
    p.set_ean(find_ean());
    p.set_holdbarhet(find_durability());
    return p;
}

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

// find the scancode (ean/PC) of the matrix code
// the manual says it starts with 01 and after my
// best knowledge all codes start with 01.
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
            rx_string = {"\\d+(\\B" + escape_code + ")"};
        } else {
            rx_string = {"(\\B" + escape_code + ")\\d+"};
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

QString FMD_decoder::remove_from_string(QString string, int fmd_size) const
{
    auto len = string.size();
    string.remove(fmd_size, len-fmd_size);

    return string;
}

// the durability is always 6 digits.
QString FMD_decoder::find_durability()
{
    using namespace apotek::constants;

    QString result = find_escape_code(escape_code_durability);
    result = remove_from_string(result, fmd_durability_size);

    return result;
}

QString FMD_decoder::find_ean()
{

    using namespace apotek::constants;

    QString result =  find_escape_code(escape_code_ean);

    // remove from ean if it starts with 0.
    if(result.startsWith(("0"))) {
        result.remove(0, 1);
    }

    result = remove_from_string(result, size_of_ean);

    return result;
}

int FMD_decoder::find_stockno()
{
    int stockno{0};
    QString ean = find_ean();

    // the stockno is in the ean after 6 numbers
    // and the last digit is not included in the stockno.
    ean.remove(0, 6);
    ean.remove(ean.size()-1, ean.size());

    stockno = ean.toInt();

    return stockno;
}

QString FMD_decoder::find_breakpoint_code(const QString& start_escape_code, const QString& end_escape_code)
{
    // not using namespace here because that will 'polute' the scope with the escape_codes set in apotek::constants.

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

} // namespace
} // namespace
