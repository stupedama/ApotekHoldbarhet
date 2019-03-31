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

CalendarWidget::CalendarWidget(int r, int v, QWidget *parent) :

    QDialog(parent),
    ui(new Ui::CalendarWidget),
    m_row{r},
    m_varenr{v}
{
    ui->setupUi(this);
}

CalendarWidget::~CalendarWidget()
{
    delete ui;
}

void CalendarWidget::on_buttonBox_accepted()
{
    emit signal_date(ui->calendarWidget->selectedDate(), m_row, m_varenr);
    this->close();
}

} // namespace
} // namespace
