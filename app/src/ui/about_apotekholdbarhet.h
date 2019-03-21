#ifndef APOTEKH_ABOUT_APOTEKHOLDBARHET_H
#define APOTEKH_ABOUT_APOTEKHOLDBARHET_H

#include <QDialog>
#include "../version.h"

namespace Ui {
class AboutApotekHoldbarhet;
}

namespace apotek {
namespace apotekholdbarhet {

class AboutApotekHoldbarhet : public QDialog
{
    Q_OBJECT

public:
    explicit AboutApotekHoldbarhet(QWidget *parent = nullptr);
    ~AboutApotekHoldbarhet();

private:
    Ui::AboutApotekHoldbarhet *ui;
};

} // namespace
} // namespace

#endif // ABOUT_APOTEKHOLDBARHET_H
