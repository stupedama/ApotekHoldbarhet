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

#include "calendar_widget.h"
#include "ui_calendarwidget.h"
#include <QDebug>
#include <QDate>

namespace apotek {
namespace apotekholdbarhet {

CalendarWidget::CalendarWidget(int row, int varenr, const QString& lokasjon, const QString& saved_date, QWidget *parent) :

    QDialog(parent),
    ui(new Ui::CalendarWidget),
    m_row{row},
    m_varenr{varenr},
    m_lokasjon{lokasjon},
    m_saved_date{saved_date}
{
    ui->setupUi(this);


    QDate date = QDate::fromString(m_saved_date, apotek::constants::date_format);
    ui->calendarWidget->setSelectedDate(date);

    ui->label_lokasjon->setText(m_lokasjon);
}

CalendarWidget::CalendarWidget(int row, int varenr, const QString& lokasjon, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CalendarWidget),
    m_row{row},
    m_varenr{varenr},
    m_lokasjon{lokasjon}
{
    ui->setupUi(this);
    ui->label_lokasjon->setText(m_lokasjon);
}

CalendarWidget::~CalendarWidget()
{
    delete ui;
}

void CalendarWidget::on_buttonBox_accepted()
{
    emit signal_date(ui->calendarWidget->selectedDate(), m_row, m_varenr, ui->label_lokasjon->text());
    this->close();
}

} // namespace
} // namespace
