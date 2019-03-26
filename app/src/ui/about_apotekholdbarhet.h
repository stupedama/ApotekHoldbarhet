#ifndef APOTEKH_ABOUT_APOTEKHOLDBARHET_H
#define APOTEKH_ABOUT_APOTEKHOLDBARHET_H

#include <QDialog>
#include <QDate>
#include <QString>
#include <chrono>
#include <date.h>
#include "../version.h"


namespace Ui {
class AboutApotekHoldbarhet;
}

namespace apotek {
namespace apotekholdbarhet {

/**
 * @brief The AboutApotekHoldbarhet class
 * Shows information about the program.
 * Who made it, where you can download it and
 * info about compilator and Qt version.
 */
class AboutApotekHoldbarhet : public QDialog
{
    Q_OBJECT

public:
    explicit AboutApotekHoldbarhet(QWidget *parent = nullptr);
    void compiled_info() const;
    QString get_compiler_version() const;
    QDate get_todays_date() const;
    QString get_qt_version() const;
    ~AboutApotekHoldbarhet();

private:
    Ui::AboutApotekHoldbarhet *ui;
};

} // namespace
} // namespace

#endif // ABOUT_APOTEKHOLDBARHET_H
