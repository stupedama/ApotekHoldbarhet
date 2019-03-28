#ifndef APOTEKH_CALENDAR_WIDGET_H
#define APOTEKH_CALENDAR_WIDGET_H

#include <QDialog>
#include <QDate>

namespace Ui {
class CalendarWidget;
}

namespace apotek {
namespace apotekholdbarhet {

/**
 * @brief The CalendarWidget class
 * The ui calendar for durability products.
 */
class CalendarWidget : public QDialog
{
    Q_OBJECT

public:
    explicit CalendarWidget(int r, int v, QWidget *parent = nullptr);
    ~CalendarWidget();

signals:
    void signal_date(QDate, int, int);
private slots:
    void on_buttonBox_accepted();

private:
    Ui::CalendarWidget *ui;
    int m_row;
    int m_varenr;
};

} // namespace
} // namespace

#endif // CALENDAR_WIDGET_H
