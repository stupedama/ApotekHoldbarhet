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
