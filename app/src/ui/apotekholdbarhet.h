#ifndef APOTEKH_APOTEKHOLDBARHET_H
#define APOTEKH_APOTEKHOLDBARHET_H

#include <QMainWindow>
#include <QString>
#include <QSqlError>
#include <QPushButton>
#include <QHBoxLayout>
#include <QTableWidgetItem>
#include <QWidget>
#include "../table_colors.h"
#include "calendar_widget.h"
#include "about_apotekholdbarhet.h"
#include "about_fest.h"
#include "print_durability.h"
#include "fest_update.h"
#include "add_vare.h"
#include "../product.h"
#include "../database.h"
#include <fest_reader.h>
#include "../version.h"
#include "../constants.h"
#include "../error_messages.h"
#include "../table_names.h"
#include "../calculate_months.h"
#include "tz.h"
#include "date.h"


namespace Ui {
class ApotekHoldbarhet;
}

namespace apotek{
namespace apotekholdbarhet{


/**
 * @brief The ApotekHoldbarhet class
 * This is the main window of the application.
 */
class ApotekHoldbarhet : public QMainWindow
{
    Q_OBJECT

public:
    explicit ApotekHoldbarhet(QWidget *parent = nullptr);
    ~ApotekHoldbarhet();
    void show_calendar(int row, int varenr) const;
    void set_title() const;
    void make_durability_table();
    void make_varer_table();
    void save_row(int r, const QString& holdbarhet);
    void delete_row(std::size_t r);
    void search_result(const std::vector<apotek::database::Product>& result);
    void fetch_products();
    void setup_table(std::size_t row_size) const;
    void remove_from_durability_vector(const apotek::database::Product& v);
    void populate_table() { make_tables(m_durability_products); }
    void populate_table(const std::vector<apotek::database::Product>& products) { make_tables(products); }
    void make_tables(const std::vector<apotek::database::Product>& products);
    TABLE_COLORS table_color(const QString& holdbarhet) const;
    void set_label_colors() const;
    void sort_durability();
    void check_for_database_error();
    int calculate_months(const QString& holdbarhet) const;
    QDate get_fest_hentetdato() const;

private slots:
    void on_actionAvslutt_triggered();
    void on_search_line_returnPressed();
    void add_date(const QDate& date, int row, int varenr, const QString& lokasjon);
    void add_newproduct(const apotek::database::Product& product);
    void on_pushButton_released();
    void on_actionAlle_varer_med_holdbarhet_triggered();
    void on_actionOm_ApotekHoldbarhet_triggered();
    void on_actionFEST_Versjon_triggered();
    void on_button_holdbarhet_overview_released();
    void on_actionOppdater_FEST_triggered();
    void on_actionLagre_ny_vare_til_databasen_triggered();
    void on_table_varer_itemDoubleClicked(QTableWidgetItem* item);

    void on_button_print_list_released();

private:
    Ui::ApotekHoldbarhet *ui;
    std::vector<apotek::database::Product> m_products;
    std::vector<apotek::database::Product> m_durability_products;
    apotek::database::Database m_db;
};

} // namespace
} // namespace

#endif // APOTEKHOLDBARHET_H
