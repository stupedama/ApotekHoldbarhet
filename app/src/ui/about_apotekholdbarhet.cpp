#include "about_apotekholdbarhet.h"
#include "ui_about_apotekholdbarhet.h"


namespace apotek {
namespace apotekholdbarhet {

AboutApotekHoldbarhet::AboutApotekHoldbarhet(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutApotekHoldbarhet)
{
    ui->setupUi(this);
    ui->textBrowser->setOpenExternalLinks(true);
    ui->textBrowser_2->setOpenExternalLinks(true);

    QString title = ui->label_title->text();
    ui->label_title->setText(title + " " + apotek::apotekholdbarhet::apotek_program_version);
}

AboutApotekHoldbarhet::~AboutApotekHoldbarhet()
{
    delete ui;
}

} // namespace
} // namespace
