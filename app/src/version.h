#ifndef APOTEKH_VERSION_H
#define APOTEKH_VERSION_H

#include <QString>

namespace apotek {
namespace apotekholdbarhet {

constexpr int MAJOR{0};
constexpr int MINOR{2};
constexpr int PATCH{0};

// version of the program
const QString apotek_program_version = "v"
        + QString::number(MAJOR)
        + "." + QString::number(MINOR)
        + "." + QString::number(PATCH);


} // namespace
} // namespace

#endif // APOTEKH_VERSION_H
