#include <QString>
#include <QRegExp>

namespace apotek {
namespace apotekholdbarhet{

bool check_numbers(const QString& s)
{
    const QRegExp check_string("\\d*");

    if(check_string.exactMatch(s)) {
        return true;
    } else {
        return false;
    }
}

} // namespace
} // namespace
