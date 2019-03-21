#include "add_vare.h"
#include "ui_add_vare.h"

namespace apotek {
namespace apotekholdbarhet {

Add_vare::Add_vare(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Add_vare)
{
    ui->setupUi(this);

}

Add_vare::~Add_vare()
{
    delete ui;
}

void Add_vare::on_button_cancel_released()
{
    this->close();
}


// TODO: make a singal slot so you can check database already in use varenr, ean.

bool Add_vare::check_varenr() const
{
    return false;
}

bool Add_vare::check_navn() const
{
    return true;
}

bool Add_vare::check_ean() const
{
    return true;
}

bool Add_vare::check_mengde() const
{
    return true;
}

bool Add_vare::check_legemiddelform() const
{
    return true;
}

void Add_vare::on_button_ok_released()
{
    if(!check_varenr()) {
        ui->error_label->setText("Vennligst fyll inn et valid varenr.");
    } else if(!check_navn()) {
        ui->error_label->setText("Vennligst fyll inn et navn.");
    } else if(!check_ean()) {
        ui->error_label->setText("Vennligst fyll inn en valid strekkode.");
    } else if(!check_mengde()) {
        ui->error_label->setText("Vennligst fyll inn en valid mengde.");
    } else if(!check_legemiddelform()) {
        ui->error_label->setText("Vennligst fyll inn en valid legemiddelform.");
    } else {
        // TODO: make signal slot so I can save this.
        this->close();
    }
}

} // namespace
} // namespace
