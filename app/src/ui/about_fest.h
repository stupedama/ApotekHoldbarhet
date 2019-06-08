#ifndef APOTEKH_ABOUT_FEST_H
#define APOTEKH_ABOUT_FEST_H

#include "fest_reader.h"
#include <QDialog>

namespace Ui {
class AboutFest;
}

namespace apotek {
namespace apotekholdbarhet {

/**
 * @brief The AboutFest class
 * Is a QDialog with information about the FEST database from Legemiddelverket.
 * it also prints the release date of the xml file. The file usally gets updated with
 * drugs and other products every 14 days.
 */
class AboutFest : public QDialog
{
    Q_OBJECT

public:
    explicit AboutFest(QString fest_hentetdato, QWidget *parent = nullptr);
    ~AboutFest();
    QString format_date(QString d) const;

private:
    Ui::AboutFest *ui;
    QString m_fest_hentetdato;
};

} // namespace
} // namespace

#endif // ABOUT_FEST_H
