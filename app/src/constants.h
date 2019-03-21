#ifndef APOTEKH_CONSTANTS_H
#define APOTEKH_CONSTANTS_H

#include <QString>
#include <QColor>

namespace apotek {
namespace constants{

// width of columns
constexpr int first_colum_width{100}; // varenr
constexpr int second_colum_width{315}; // varenavn
constexpr int third_colum_width{70}; // mengde
constexpr int forth_colum_width{130}; // legemiddelform
constexpr int fifth_colum_width{230}; // holdbarhet
constexpr int sixth_colum_width{200}; // lokasjon
constexpr int seventh_colum_width{100}; // lagre
constexpr int eight_colum_width{95}; // slett

// naming of tables
const QString first_colum_name{"Varenr"};
const QString second_colum_name{"Varenavn"};
const QString third_colum_name{"Mengde"};
const QString forth_colum_name{"Legemiddelform"};
const QString fifth_colum_name{"Holdbarhet"};
const QString sixth_colum_name{"Lokasjon"};
const QString seventh_colum_name{"Lagre"};
const QString eight_colum_name{"Slett"};

// color of tables
const QColor color_orange{255, 165, 0};
const QColor color_red{178, 34, 34};
const QColor color_blue{30, 144, 255};
const QColor color_white{255, 255, 255};
const QColor color_yellow{255, 255, 0};

} // namespace
} // namespace

#endif // CONSTANTS_H
