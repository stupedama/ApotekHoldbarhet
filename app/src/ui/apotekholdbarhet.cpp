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

#include "apotekholdbarhet.h"
#include "ui_apotekholdbarhet.h"
#include "../apotekholdbarhet_init.h"

namespace apotek{
namespace apotekholdbarhet{

ApotekHoldbarhet::ApotekHoldbarhet(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ApotekHoldbarhet)
{

    using namespace apotek;
    using namespace database;

    ui->setupUi(this);

    // sets windows fixed size, title, gets all the products and label colors
    apotekholdbarhet::apotekholdbarhet_init(*this);

    // make an empty vector so we can call our make_table()
    std::vector<Product> empty_products;

    if(m_products.empty())
        ui->statusBar->showMessage(apotek::errors::error_no_products);

    const auto max_value = std::numeric_limits<int>::max();

    // TODO: make it so it can only list maximum values in the table, but you can search in the database for other varer.
    // FIXME: this needs to be checked everytime in make_holdbarhet_vare() and populate_table(). Otherwise disaster.

    // vector size is in 'std::size_t', but Qt table rows expects 'int'.
    // we are making a static_cast<int> - if the vectors size is over int maximum values, the program will have undefined behaviour.
    if(m_durability_products.size() >= max_value - 10) {
        ui->statusBar->showMessage(apotek::errors::error_vector_size_soon);
        make_durability_table();
    } else if(m_durability_products.size() == max_value) {
        ui->statusBar->showMessage(apotek::errors::error_vector_size);
    } else {
        make_durability_table();
    }

    // notify about error.
    check_for_database_error();
}


// checks the database for error messages
// this should be called last in the function that it is needed in.
void ApotekHoldbarhet::check_for_database_error()
{
    if(!m_db.get_error_status().isEmpty()) {
        ui->statusBar->showMessage(m_db.get_error_status());

        // clear the error, we already have notifed the user about it.
        m_db.set_error_status("");
    }
}

void ApotekHoldbarhet::set_label_colors() const
{
    using namespace apotek;

    QString background_color{"background-color: " };
    QString red_background = background_color + constants::color_red.name();
    QString yellow_background = background_color + constants::color_yellow.name();
    QString orange_background = background_color + constants::color_orange.name();
    QString white_background = background_color + constants::color_white.name();

    ui->label_hold_0_3->setStyleSheet(red_background);
    ui->label_hold_4_6->setStyleSheet(orange_background);
    ui->label_hold_7_12->setStyleSheet(yellow_background);
    ui->label_hold_12->setStyleSheet(white_background);
}

// Sorts m_holdbarhet_varer after its expirity date (holdbarhet)
void ApotekHoldbarhet::sort_durability()
{
    std::sort(std::begin(m_durability_products), std::end(m_durability_products), [&](
              const apotek::database::Product& lhs,
              const apotek::database::Product& rhs) -> bool
    {
        // TODO: make the colors::BLUE last.
        //if(lhs.get_holdbarhet().isEmpty()) return false;
        //if(rhs.get_holdbarhet().isEmpty()) return false;

        return calculate_months(lhs.get_holdbarhet()) < calculate_months(rhs.get_holdbarhet());
    });
}

// fetches all the varer from the database
// it fetches all the varer and holdbarhet_varer is in the database, if theres no
// varer it fetches them from the fest xml file.
// it also checks if the 'vare' is already saved in the database, otherwise it gets saved.
void ApotekHoldbarhet::fetch_products()
{
    m_products = m_db.get_products();
    m_durability_products = m_db.get_durability();

    check_for_database_error();
}

ApotekHoldbarhet::~ApotekHoldbarhet()
{
    delete ui;
}

void ApotekHoldbarhet::set_title() const
{
    ui->label_version->setText(apotek_program_version);
}

void ApotekHoldbarhet::on_actionAvslutt_triggered()
{
    ApotekHoldbarhet::close();
}

// setups the table shell in QTableWidget.
void ApotekHoldbarhet::setup_table(std::size_t row_size) const
{
    using namespace apotek;
    // clear the table.
    ui->table_varer->setRowCount(0);

    ui->table_varer->setHorizontalHeaderItem(VARENR, new QTableWidgetItem(constants::first_colum_name));
    ui->table_varer->setHorizontalHeaderItem(VARENAVN, new QTableWidgetItem(constants::second_colum_name));
    ui->table_varer->setHorizontalHeaderItem(MENGDE, new QTableWidgetItem(constants::third_colum_name));
    ui->table_varer->setHorizontalHeaderItem(LEGEMIDDELFORM, new QTableWidgetItem(constants::forth_colum_name));
    ui->table_varer->setHorizontalHeaderItem(HOLDBARHET, new QTableWidgetItem(constants::fifth_colum_name));
    ui->table_varer->setHorizontalHeaderItem(LOKASJON, new QTableWidgetItem(constants::sixth_colum_name));
    ui->table_varer->setHorizontalHeaderItem(LAGRE, new QTableWidgetItem(constants::seventh_colum_name));
    ui->table_varer->setHorizontalHeaderItem(SLETT, new QTableWidgetItem(constants::eight_colum_name));
    ui->table_varer->setRowCount(static_cast<int>(row_size));

    // set the size of colums
    ui->table_varer->setColumnWidth(VARENR, constants::first_colum_width);
    ui->table_varer->setColumnWidth(VARENAVN, constants::second_colum_width);
    ui->table_varer->setColumnWidth(MENGDE, constants::third_colum_width);
    ui->table_varer->setColumnWidth(LEGEMIDDELFORM, constants::forth_colum_width);
    ui->table_varer->setColumnWidth(HOLDBARHET, constants::fifth_colum_width);
    ui->table_varer->setColumnWidth(LOKASJON, constants::sixth_colum_width);
    ui->table_varer->setColumnWidth(LAGRE, constants::seventh_colum_width);
    ui->table_varer->setColumnWidth(SLETT, constants::eight_colum_width);
}

// populates the QTableWidget with varer
void ApotekHoldbarhet::populate_table(const std::vector<apotek::database::Product>& result)
{
    using namespace apotek;

    sort_durability();
    // clear the error message
    ui->error_message->setText("");

    auto row_size = result.size();

    for(std::size_t r{0}; r < row_size; ++r) {
        // make som buttons
        // save and delete
        QPushButton* calendar_button = new QPushButton;
        QHBoxLayout* calendar_layout = new QHBoxLayout;
        QWidget* calendar_widget = new QWidget;

        QPushButton* save_button = new QPushButton;
        QHBoxLayout* save_layout = new QHBoxLayout;
        QWidget* save_widget = new QWidget;

        QPushButton* delete_button = new QPushButton;
        QHBoxLayout* delete_layout = new QHBoxLayout;
        QWidget* delete_widget = new QWidget;

        calendar_layout->addWidget(calendar_button);
        calendar_widget->setLayout(calendar_layout);

        save_button->setText("X");
        save_layout->addWidget(save_button);
        save_widget->setLayout(save_layout);
        save_widget->setStyleSheet("background-color: white");

        delete_button->setText("X");
        delete_layout->addWidget(delete_button);
        delete_widget->setLayout(delete_layout);
        delete_widget->setStyleSheet("background-color: white");

        ui->table_varer->setItem(static_cast<int>(r), VARENR, new QTableWidgetItem(QString::number(result[r].get_varenr())));
        ui->table_varer->setItem(static_cast<int>(r), VARENAVN, new QTableWidgetItem(result[r].get_navn()));
        ui->table_varer->setItem(static_cast<int>(r), MENGDE, new QTableWidgetItem(QString::number(result[r].get_mengde())));
        ui->table_varer->setItem(static_cast<int>(r), LEGEMIDDELFORM, new QTableWidgetItem(result[r].get_legemiddelform()));

        ui->table_varer->setCellWidget(static_cast<int>(r), HOLDBARHET, calendar_widget);
          ui->table_varer->setItem(static_cast<int>(r), LOKASJON, new QTableWidgetItem(result[r].get_lokasjon()));
        ui->table_varer->setCellWidget(static_cast<int>(r), LAGRE, save_widget);

        //ui->table_varer->setCellWidget(static_cast<int>(r), SLETT, delete_widget);

        if(!result[r].get_holdbarhet().isEmpty()) {
            QLabel* calendar_label = new QLabel;
            table_color(result[r].get_holdbarhet());
            calendar_label->setText(result[r].get_holdbarhet());
            calendar_layout->addWidget(calendar_label);
            calendar_label->setText(result[r].get_holdbarhet());
            calendar_widget->setStyleSheet("background-color: white");
        }

        auto color = table_color(result[r].get_holdbarhet());

        switch(color) {
        case TABLE_COLORS::RED:
            ui->table_varer->item(static_cast<int>(r), VARENR)->setBackgroundColor(constants::color_red);
            ui->table_varer->item(static_cast<int>(r), VARENAVN)->setBackgroundColor(constants::color_red);
            ui->table_varer->item(static_cast<int>(r), MENGDE)->setBackgroundColor(constants::color_red);
            ui->table_varer->item(static_cast<int>(r), LEGEMIDDELFORM)->setBackgroundColor(constants::color_red);
            break;
        case TABLE_COLORS::YELLOW:
            ui->table_varer->item(static_cast<int>(r), VARENR)->setBackgroundColor(constants::color_yellow);
            ui->table_varer->item(static_cast<int>(r), VARENAVN)->setBackgroundColor(constants::color_yellow);
            ui->table_varer->item(static_cast<int>(r), MENGDE)->setBackgroundColor(constants::color_yellow);
            ui->table_varer->item(static_cast<int>(r), LEGEMIDDELFORM)->setBackgroundColor(constants::color_yellow);
            break;
        case TABLE_COLORS::ORANGE:
            ui->table_varer->item(static_cast<int>(r), VARENR)->setBackgroundColor(constants::color_orange);
            ui->table_varer->item(static_cast<int>(r), VARENAVN)->setBackgroundColor(constants::color_orange);
            ui->table_varer->item(static_cast<int>(r), MENGDE)->setBackgroundColor(constants::color_orange);
            ui->table_varer->item(static_cast<int>(r), LEGEMIDDELFORM)->setBackgroundColor(constants::color_orange);
            break;
        default:
            ui->table_varer->item(static_cast<int>(r), VARENR)->setBackgroundColor(constants::color_white);
            ui->table_varer->item(static_cast<int>(r), VARENAVN)->setBackgroundColor(constants::color_white);
            ui->table_varer->item(static_cast<int>(r), MENGDE)->setBackgroundColor(constants::color_white);
            ui->table_varer->item(static_cast<int>(r), LEGEMIDDELFORM)->setBackgroundColor(constants::color_white);
            break;
        }

        // make the calendar button look nice
        calendar_button->setIcon(QIcon(":/icons/icons/calendar.png"));
        QString button_style = "QPushButton{border:none;background-color:rgba(255, 255, 255,100);}";
        calendar_button->setStyleSheet(button_style);
        calendar_button->setIconSize(QSize(50,50));
        calendar_widget->setStyleSheet("background-color: white");

        // save
        connect(save_button, &QPushButton::clicked, [=]{
            save_row(static_cast<int>(r), "none");
        });

        // calendar
        connect(calendar_button, &QPushButton::clicked, [=]{
            show_calendar(static_cast<int>(r), result[r].get_varenr());
        });

        // delete
        if(!result[r].get_holdbarhet().isEmpty()) {
            auto d = connect(delete_button, &QPushButton::clicked, [r, this]() -> void {
                this->delete_row(r);
            });
        }
    }
}

void ApotekHoldbarhet::search_result(const std::vector<database::Product> &result)
{
    using namespace apotek;

    std::size_t row_size = result.size();

    if(!result.empty()) {
        setup_table(row_size);
        populate_table(result);
    }
}

int ApotekHoldbarhet::calculate_months(const QString& holdbarhet) const
{
    using namespace date;

    sys_days today = year_month_day{floor<days>(std::chrono::system_clock::now())};
    QDate holdbarhet_date = QDate::fromString(holdbarhet, apotek::constants::date_format);

    sys_days holdbarhet_sysdays = sys_days{days{holdbarhet_date.toJulianDay()} -
            (sys_days{1970_y/jan/1} - sys_days{year{-4713}/nov/24})};

    return round<months>(holdbarhet_sysdays-today).count();
}

// difference in months from today and the QString holdbarhet.
TABLE_COLORS ApotekHoldbarhet::table_color(const QString& holdbarhet) const
{
    if(holdbarhet.isEmpty()) return TABLE_COLORS::BLUE;

    int m = calculate_months(holdbarhet);

    if(m <= 3) {
        return TABLE_COLORS::RED;
    } else if(m > 3 && m <= 6) {
        return TABLE_COLORS::ORANGE;
    } else if(m > 6 && m <= 12) {
        return TABLE_COLORS::YELLOW;
    } else {
        return TABLE_COLORS::WHITE;
    }
}

// Opens the calendar and it returns the selected date
// it also saves the date to the vector and the database.
void ApotekHoldbarhet::show_calendar(int row, int varenr=0) const
{
    QString lokasjon =  ui->table_varer->item(row, LOKASJON)->text();

    CalendarWidget* calendar = new CalendarWidget(row, varenr, lokasjon);
    calendar->setAttribute(Qt::WA_DeleteOnClose);

    // TODO: need to change name of function as it now saves two things.
    connect(calendar,SIGNAL(signal_date(QDate, int, int, QString)),this,SLOT(add_date(QDate, int, int, QString)));
    calendar->show();
}

// FIXME: Holdbarhetsvare doesnt update, it adds a new vare insted. Maybe look in the ApotekHoldbarhet::search_results ?
void ApotekHoldbarhet::add_date(const QDate& qd, int row, int varenr, const QString& lokasjon)
{
    using namespace date;

    // we are passing varenr 0 when we are searching for products,
    // otherwise we are passing the varenr.
    // TODO: need to change this algorithm.
    if(varenr == 0) {
        QString string_date = qd.toString(apotek::constants::date_format);
        QString string_varenr = ui->table_varer->item(row, VARENR)->text();
        varenr = string_varenr.toInt();

        //std::find_if(std::begin(m_durability_products), std)

        //auto product = m_durability_products[static_cast<std::size_t>(row)];
        auto search_result = m_db.search_product(string_varenr);

        if(!search_result.empty()) {
            search_result[0].set_holdbarhet(string_date);
            search_result[0].set_lokasjon(lokasjon);

            m_db.save_durability(search_result[0]);

            // remove the old one vare in the vector and save the new one.
            remove_from_durability_vector(search_result[0]);

            // save the modified one
            m_durability_products.push_back(search_result[0]);

            // print out the new holdbarhetsvare tabel
            make_durability_table();
        }
    } else {
        QString search_product = QString::number(varenr);
        std::vector<apotek::database::Product> search_result = m_db.search_product(search_product);

        if(search_result.empty()) {
            make_durability_table();
        } else {
            // the result should only return one
            auto product = search_result[0];

            QTableWidgetItem* item_lokasjon(ui->table_varer->item(0, LOKASJON));

            if(item_lokasjon) {
                QString lokasjon = ui->table_varer->item(0, LOKASJON)->text();
                product.set_lokasjon(lokasjon);
            }

            QString string_date = qd.toString(apotek::constants::date_format);
            product.set_holdbarhet(string_date);
            product.set_lokasjon(lokasjon);

            m_db.save_durability(product);

            // save the modified one
            m_durability_products.push_back(product);

            // print out the new holdbarhetsvare tabel
            make_durability_table();
        }
    }

    // check for errors.
    check_for_database_error();
}

// TODO: Make a generic populate_table, so it can populate varer and holdbarhet.
// probably bad design. Need to only make one populate_table and make_holdbarhet_table.
// FIXME: need to make this using populate_table, make populate_tabke more generic.
void ApotekHoldbarhet::make_durability_table()
{
    using namespace apotek;
    using namespace apotekholdbarhet;

    sort_durability();
    // TODO: possible bug/overflow if the vector is bigger than a signed 32 bit int.
    // make a guard (?)

    // clear the error message
    ui->error_message->setText("");

    std::size_t row_size = m_durability_products.size();
    setup_table(row_size);
    //populate_table(m_holdbarhet_varer);

    // populate our table
    for(std::size_t r{0}; r < row_size; ++r) {
        // make som buttons
        // save and delete calendar
        QPushButton* calendar_button = new QPushButton();
        QHBoxLayout* calendar_layout = new QHBoxLayout;
        QWidget* calendar_widget = new QWidget;

        QPushButton* save_button = new QPushButton();
        QHBoxLayout* save_layout = new QHBoxLayout;
        QWidget* save_widget = new QWidget;

        QPushButton* delete_button = new QPushButton;
        QHBoxLayout* delete_layout = new QHBoxLayout;
        QWidget* delete_widget = new QWidget;

        save_button->setText("X");
        save_layout->addWidget(save_button);
        save_widget->setLayout(save_layout);
        save_widget->setStyleSheet("background-color: white");

        delete_button->setText("X");
        delete_layout->addWidget(delete_button);
        delete_widget->setLayout(delete_layout);
        delete_widget->setStyleSheet("background-color: white");

        if(!m_durability_products[r].get_holdbarhet().isEmpty() && !m_durability_products.empty()) {
            QLabel* calendar_label = new QLabel;
            table_color(m_durability_products[r].get_holdbarhet());
            calendar_label->setText(m_durability_products[r].get_holdbarhet());
            calendar_layout->addWidget(calendar_label);
            calendar_label->setText(m_durability_products[r].get_holdbarhet());
        }

        calendar_layout->addWidget(calendar_button);
        calendar_widget->setLayout(calendar_layout);
        calendar_widget->setStyleSheet("background-color: white");

        ui->table_varer->setItem(static_cast<int>(r), VARENR, new QTableWidgetItem(QString::number(m_durability_products[r].get_varenr())));
        ui->table_varer->setItem(static_cast<int>(r), VARENAVN, new QTableWidgetItem(m_durability_products[r].get_navn()));
        ui->table_varer->setItem(static_cast<int>(r), MENGDE, new QTableWidgetItem(QString::number(m_durability_products[r].get_mengde())));
        ui->table_varer->setItem(static_cast<int>(r), LEGEMIDDELFORM, new QTableWidgetItem(m_durability_products[r].get_legemiddelform()));
        ui->table_varer->setCellWidget(static_cast<int>(r), HOLDBARHET, calendar_widget);
        ui->table_varer->setItem(static_cast<int>(r), LOKASJON, new QTableWidgetItem(m_durability_products[r].get_lokasjon()));
        ui->table_varer->setCellWidget(static_cast<int>(r), LAGRE, save_widget);
        ui->table_varer->setCellWidget(static_cast<int>(r), SLETT, delete_widget);

        auto color = table_color(m_durability_products[r].get_holdbarhet());

        switch(color) {
        case TABLE_COLORS::RED:
            ui->table_varer->item(static_cast<int>(r), VARENR)->setBackgroundColor(constants::color_red);
            ui->table_varer->item(static_cast<int>(r), VARENAVN)->setBackgroundColor(constants::color_red);
            ui->table_varer->item(static_cast<int>(r), MENGDE)->setBackgroundColor(constants::color_red);
            ui->table_varer->item(static_cast<int>(r), LEGEMIDDELFORM)->setBackgroundColor(constants::color_red);
            break;
        case TABLE_COLORS::BLUE:
            ui->table_varer->item(static_cast<int>(r), VARENR)->setBackgroundColor(constants::color_blue);
            ui->table_varer->item(static_cast<int>(r), VARENAVN)->setBackgroundColor(constants::color_blue);
            ui->table_varer->item(static_cast<int>(r), MENGDE)->setBackgroundColor(constants::color_blue);
            ui->table_varer->item(static_cast<int>(r), LEGEMIDDELFORM)->setBackgroundColor(constants::color_blue);

            // set tooltip
            //ui->table_varer->item(static_cast<int>(r), )

            break;
        case TABLE_COLORS::YELLOW:
            ui->table_varer->item(static_cast<int>(r), VARENR)->setBackgroundColor(constants::color_yellow);
            ui->table_varer->item(static_cast<int>(r), VARENAVN)->setBackgroundColor(constants::color_yellow);
            ui->table_varer->item(static_cast<int>(r), MENGDE)->setBackgroundColor(constants::color_yellow);
            ui->table_varer->item(static_cast<int>(r), LEGEMIDDELFORM)->setBackgroundColor(constants::color_yellow);
            break;
        case TABLE_COLORS::ORANGE:
            ui->table_varer->item(static_cast<int>(r), VARENR)->setBackgroundColor(constants::color_orange);
            ui->table_varer->item(static_cast<int>(r), VARENAVN)->setBackgroundColor(constants::color_orange);
            ui->table_varer->item(static_cast<int>(r), MENGDE)->setBackgroundColor(constants::color_orange);
            ui->table_varer->item(static_cast<int>(r), LEGEMIDDELFORM)->setBackgroundColor(constants::color_orange);
            break;
        default:
            ui->table_varer->item(static_cast<int>(r), VARENR)->setBackgroundColor(constants::color_white);
            ui->table_varer->item(static_cast<int>(r), VARENAVN)->setBackgroundColor(constants::color_white);
            ui->table_varer->item(static_cast<int>(r), MENGDE)->setBackgroundColor(constants::color_white);
            ui->table_varer->item(static_cast<int>(r), LEGEMIDDELFORM)->setBackgroundColor(constants::color_white);
            break;
        }

        // make the calendar button look nice
        calendar_button->setIcon(QIcon(":/icons/icons/calendar.png"));
        QString button_style = "QPushButton{border:none;background-color:rgba(255, 255, 255,100);}";
        calendar_button->setStyleSheet(button_style);
        calendar_button->setIconSize(QSize(50,50));

        // calendar
        connect(calendar_button, &QPushButton::clicked, [=]{
            show_calendar(static_cast<int>(r));
        });

        // save
        connect(save_button, &QPushButton::clicked, [=]{
            save_row(static_cast<int>(r), m_durability_products[r].get_holdbarhet());
        });

        // delete
        auto d = connect(delete_button, &QPushButton::clicked, [r, this]() -> void {
            this->delete_row(r);
        });
    }
}


// saves the vare based on the row in QTableWidget.
void ApotekHoldbarhet::save_row(int r, const QString& holdbarhet)
{
    QString varenr = ui->table_varer->item(r, VARENR)->text();

    std::vector<apotek::database::Product> product_vector = m_db.search_product(varenr);

    for(auto& product : product_vector) {

        QString lokasjon;
        int mengde;
        QString legemiddelform;

        if(!holdbarhet.contains("none")) {
            product.set_holdbarhet(holdbarhet);
        }

        QTableWidgetItem* item_mengde(ui->table_varer->item(r, MENGDE));
        if(item_mengde) {
            QString string_mengde = ui->table_varer->item(r, MENGDE)->text();
            mengde = string_mengde.toInt();
            product.set_mengde(mengde);
        }

        QTableWidgetItem* item_legemiddelform(ui->table_varer->item(r, LEGEMIDDELFORM));
        if(item_legemiddelform) {
            legemiddelform = ui->table_varer->item(r, LEGEMIDDELFORM)->text();
            product.set_legemiddelform(legemiddelform);
        }

        // this returns null if the cell is empty
        QTableWidgetItem* item_lokasjon(ui->table_varer->item(r, LOKASJON));
        if(item_lokasjon) {
            lokasjon = ui->table_varer->item(r, LOKASJON)->text();
            product.set_lokasjon(lokasjon);
        }

        m_db.save_durability(product);

        // remove the old one vare in the vector and save the new one.
        remove_from_durability_vector(product);

        // save the modified one
        m_durability_products.push_back(product);
    }
    // print out the new holdbarhetsvare tabel
    make_durability_table();

    // check for errors
    check_for_database_error();
}


void ApotekHoldbarhet::remove_from_durability_vector(const apotek::database::Product& v)
{
    auto i = std::find_if(std::begin(m_durability_products), std::end(m_durability_products),
                          apotek::database::Product::Find_vare(v.get_varenr()));

    if(i != std::end(m_durability_products)) {
        m_durability_products.erase(i);
    }
}

// deletes the row and updates the vector
// clears the table and prints out the modified vector by 'void make_holdbarhet_table()'.
void ApotekHoldbarhet::delete_row(std::size_t r)
{
    auto i = std::find_if(std::begin(m_durability_products), std::end(m_durability_products), [this, r](
                          const apotek::database::Product& v)
    {
        return v.get_varenr() == m_durability_products[r].get_varenr();
    });

    if(i != std::end(m_durability_products)) {
        // remove from database
        m_db.remove_durability(*i);
        // remove from our ApotekHoldbarhet::m_holdbarhet_varer
        m_durability_products.erase(i);
        // clear old table and post it again
        make_durability_table();
    }
}

void ApotekHoldbarhet::on_search_line_returnPressed()
{
    if(ui->search_line->text().isEmpty()) {
        ui->error_message->setText(errors::error_ui_no_result);
        return;
    }

    QString search_value = ui->search_line->text();
    std::vector<apotek::database::Product> result = m_db.search_product(search_value);

    if(result.empty()) {
        ui->error_message->setText(apotek::errors::error_ui_no_product);
    } else {
        search_result(result);
        ui->search_line->clear();
    }
}

void ApotekHoldbarhet::on_pushButton_released()
{
    on_search_line_returnPressed();
}

void ApotekHoldbarhet::on_actionAlle_varer_med_holdbarhet_triggered()
{
    on_button_holdbarhet_overview_released();
}

void ApotekHoldbarhet::on_actionOm_ApotekHoldbarhet_triggered()
{
    AboutApotekHoldbarhet* about = new AboutApotekHoldbarhet();
    about->setAttribute(Qt::WA_DeleteOnClose);
    about->show();
}

void ApotekHoldbarhet::on_actionFEST_Versjon_triggered()
{

    QString fest_hentetdato = m_db.get_fest_hentetdato();

    AboutFest* about = new AboutFest(fest_hentetdato);
    about->setAttribute(Qt::WA_DeleteOnClose);
    about->show();
}

void ApotekHoldbarhet::on_button_holdbarhet_overview_released()
{
    m_durability_products = m_db.get_durability();
    make_durability_table();
}


void ApotekHoldbarhet::on_actionOppdater_FEST_triggered()
{
    FEST_Update* fest_update = new FEST_Update();
    fest_update->setAttribute(Qt::WA_DeleteOnClose);
    fest_update->show();
}

void apotek::apotekholdbarhet::ApotekHoldbarhet::on_actionLagre_ny_vare_til_databasen_triggered()
{
    Add_vare* add_vare = new Add_vare();
    add_vare->setAttribute(Qt::WA_DeleteOnClose);
    add_vare->show();

    connect(add_vare,SIGNAL(signal_newproduct(const apotek::database::Product&)),this,SLOT(add_newproduct(const apotek::database::Product&)));
}


void ApotekHoldbarhet::add_newproduct(const apotek::database::Product& product)
{
    auto result = m_db.search_product(QString::number(product.get_varenr()));
    if(result.empty()) {
        if(m_db.save_newproduct(product)) {
            ui->statusBar->showMessage(product.get_navn() + " ble lagt til i databasen.");
        } else {
            ui->statusBar->showMessage("Det skjedde en feil!");
        }
    } else {
        ui->statusBar->showMessage(product.get_navn() + " finnes allerede i database.");
    }
}

void ApotekHoldbarhet::on_table_varer_itemDoubleClicked(QTableWidgetItem *item)
{
    show_calendar(static_cast<int>(item->row()));
}

} // namespace
} // namespace
