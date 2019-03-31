#ifndef APOTEKH_FMD_2DCODE_H
#define APOTEKH_FMD_2DCODE_H

#include <QString>
#include <QRegExp>
#include <QStringList>
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
    QString find_escape_code(const QString& escape_code);
    QString remove_from_string(QString string, int fmd_size) const;
    int find_varenr() const;
    QString m_matrix_code;
    QString m_error_code;
};

} // namespace
} // namespace

#endif // FMD_2DCODE_H
