/*
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.

   Author: Fredrik Fjeldvær (fredrik@ffj.no)
*/

#include "add_vare.h"
#include "ui_add_vare.h"

#include <QDebug>

// TODO: make a singal slot so you can check database already in use varenr, ean.

namespace apotek {
namespace apotekholdbarhet {

Add_vare::Add_vare(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Add_vare),
    m_state_varenr{false},
    m_state_navn{false},
    m_state_mengde{false},
    m_state_ean{false},
    m_state_legemiddelform{false}
{
    ui->setupUi(this);
    m_product = std::make_shared<apotek::database::Product>(apotek::database::Product());
}

Add_vare::~Add_vare()
{
    delete ui;
}

void Add_vare::on_button_cancel_released()
{
    this->close();
}

// checks the different inputlabels if its have valid input.
void Add_vare::check_state() const
{
    QString error_stylesheet{"border: 2px solid red;"};
    QString good_stylesheet{"border: 1px solid grey"};

    if(!get_state_varenr()) ui->line_varenr->setStyleSheet(error_stylesheet);
    else ui->line_varenr->setStyleSheet(good_stylesheet);

    if(!get_state_navn()) ui->line_navn->setStyleSheet(error_stylesheet);
    else ui->line_navn->setStyleSheet(good_stylesheet);

    if(!get_state_mengde()) ui->line_mengde->setStyleSheet(error_stylesheet);
    else ui->line_mengde->setStyleSheet(good_stylesheet);

    if(!get_state_ean()) ui->line_ean->setStyleSheet(error_stylesheet);
    else ui->line_ean->setStyleSheet(good_stylesheet);

    if(!get_state_legemiddelform()) ui->line_legemiddelform->setStyleSheet(error_stylesheet);
    else ui->line_legemiddelform->setStyleSheet(good_stylesheet);
}

// checks if the the QString only contains a number or only characeters, or an empty string
// based on the boolean.
bool Add_vare::check_field(const QString& check, bool number) const
{
    using apotek::apotekholdbarhet::check_numbers;

    if(number) {
        if(!check_numbers(check)) {
            ui->error_label->setText(errors::error_ui_addvare_onlynumbers);
            return false;
        }
    } else {
        if(check_numbers(check)) {
            ui->error_label->setText(errors::error_ui_addvare_onlychars);
            return false;
        }
    }

    if(check.isEmpty()) {
        ui->error_label->setText(errors::error_ui_addvare_empty);
        return false;
    }


    return true;
}

void Add_vare::clear_error() const
{
    ui->error_label->setText("");
}

void Add_vare::add_varenr(const QString& varenr)
{
    using apotek::apotekholdbarhet::check_numbers;

    if(!check_field(varenr, true)) {
        set_state_varenr(false);
        check_state();
    } else {
        m_product->set_varenr(varenr.toInt());
        set_state_varenr(true);
        clear_error();
    }
}

void Add_vare::add_navn(const QString& navn)
{
    if(!check_field(navn, false)) {
        set_state_navn(false);
        check_state();
    } else {
        m_product->set_navn(navn);
        set_state_navn(true);
        clear_error();
    }
}

void Add_vare::add_ean(const QString& ean)
{
    if(!check_field(ean, true)) {
        set_state_ean(false);
        check_state();
    } else {
        m_product->set_ean(ean);
        set_state_ean(true);
        clear_error();
    }
}

void Add_vare::add_mengde(const QString& mengde)
{

    if(!check_field(mengde, true)) {
        set_state_mengde(false);
        check_state();
    } else {

        m_product->set_mengde(mengde.toInt());
        set_state_mengde(true);
        clear_error();
    }
}

void Add_vare::add_legemiddelform(const QString& legemiddelform)
{
    if(!check_field(legemiddelform, false)) {
        set_state_legemiddelform(false);
        check_state();
    } else {
        m_product->set_legemiddelform(legemiddelform);
        set_state_legemiddelform(true);
        clear_error();
    }
}

void Add_vare::on_button_ok_released()
{
    const QString varenr = ui->line_varenr->text();
    const QString mengde = ui->line_mengde->text();
    const QString navn = ui->line_navn->text();
    const QString ean = ui->line_ean->text();
    const QString legemiddelform = ui->line_legemiddelform->text();

    add_varenr(varenr);
    add_mengde(mengde);
    add_navn(navn);
    add_ean(ean);
    add_legemiddelform(legemiddelform);

    // check if all labels are correctly filled in
    check_state();

    if(m_product->sanity_check()) {
        emit signal_newproduct(m_product);
        close();
    } else {
        ui->error_label->setText(errors::error_ui_product_invalid);
    }
}

} // namespace
} // namespace
