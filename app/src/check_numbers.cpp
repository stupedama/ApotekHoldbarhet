#include <QString>
#include <QRegExp>

namespace apotek {
namespace apotekholdbarhet{

// checks if the QString is a number or not
// returns true if string is only numbers, otherwise returns false.
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
