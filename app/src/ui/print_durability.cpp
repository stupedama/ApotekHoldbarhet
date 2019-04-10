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

#include "print_durability.h"
#include "ui_print_durability.h"

namespace apotek {
namespace apotekholdbarhet {

Print_durability::Print_durability(const std::vector<apotek::database::Product>& products, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Print_durability),
    m_products{products}
{
    ui->setupUi(this);
}

Print_durability::~Print_durability()
{
    delete ui;
}

QString Print_durability::make_top_html() const
{
    QString top_html = {
        "<html>\n"
        "<head>\n"
        "<meta Content=\"Text/html; charset=Windows-1251\">\n"
        "<title>Utskrift av holdbarhetslisten</title>"
        "</head>"
        "<body>"
    };

    return top_html;
}

QString Print_durability::make_title_html() const
{
    return "<h1>Holdbarhetsvarer</h1><br>";
}

QString Print_durability::make_table_html() const
{
    QString tables;
    tables += "<table width='100%' border='1'>";
    tables += "<tr>";
    // columns
    tables += "<th>Varenr</th>";
    tables += "<th>Navn</th>";
    tables += "<th>Mengde</th>";
    tables += "<th>Legemiddelform</th>";
    tables += "<th>Holdbarhet</th>";
    tables += "<th>Lokasjon</th>";
    tables += "</tr>";

    for(const auto& p : m_print_products) {
        tables += "<tr>"; // start

        tables += "<td>" + QString::number(p.get_varenr()) + "</td>";
        tables += "<td>" + p.get_navn() + "</td>";
        tables += "<td>" + QString::number(p.get_mengde()) + "</td>";
        tables += "<td>" + p.get_legemiddelform() + "</td>";
        tables += "<td>" + p.get_holdbarhet() + "</td>";
        tables += "<td>" + p.get_lokasjon() + "</td>";

        tables += "<\tr>"; // end
    }

    tables += "</table>";
    return tables;
}

QString Print_durability::make_bottom_html() const
{
    return "</body></html>";
}

QString Print_durability::make_html() const
{
    QString html =
              make_top_html()
            + make_title_html()
            + make_table_html()
            + make_bottom_html();

    return html;
}

void Print_durability::add_color_red()
{
    using namespace apotek;

    for(const auto& p : m_products) {
        int m = apotekholdbarhet::calculate_months(p.get_holdbarhet());
        if(m <= constants::color_interval_red) {
            m_print_products.push_back(p);
        }
    }
}

void Print_durability::add_color_orange()
{
    using namespace apotek;

    for(const auto& p : m_products) {
        int m = apotekholdbarhet::calculate_months(p.get_holdbarhet());
        if(m > constants::color_interval_red && m <= constants::color_interval_orange) {
            m_print_products.push_back(p);
        }
    }
}

void Print_durability::add_color_yellow()
{
    using namespace apotek;

    for(const auto& p : m_products) {
        int m = apotekholdbarhet::calculate_months(p.get_holdbarhet());
        if(m > constants::color_interval_orange && m <= constants::color_interval_yellow) {
            m_print_products.push_back(p);
        }
    }
}

void Print_durability::add_color_white()
{
    using namespace apotek;

    for(const auto& p : m_products) {
        int m = apotekholdbarhet::calculate_months(p.get_holdbarhet());
        if(m >= constants::color_interval_yellow) {
            m_print_products.push_back(p);
        }
    }
}

void Print_durability::print() const
{
    QString strStream;
    QTextStream out(&strStream);

    // make html page for printing
    out << make_html();

    QTextDocument* document = new QTextDocument();
    document->setHtml(strStream);

    QPrinter printer;
    QPrintDialog *dialog = new QPrintDialog(&printer);

    if (dialog->exec() == QDialog::Accepted) {
        document->print(&printer);
    }

}

void Print_durability::on_buttonBox_accepted()
{
    if(ui->box_all->isChecked()) {
        for(const auto& p : m_products) {
            m_print_products.push_back(p);
        }
        print();

        // print everything, we are done.
        return;
    }

    if(ui->box_red->isChecked()) {
        add_color_red();
    }

    if(ui->box_orange->isChecked()) {
        add_color_orange();
    }

    if(ui->box_yellow->isChecked()) {
        add_color_yellow();
    }

    if(ui->box_white->isChecked()) {
        add_color_white();
    }

    if(!m_print_products.empty()) {
        print();
    } else {
        QMessageBox::warning(
                    this,
                    tr("ApotekHoldbarhet"),
                    tr("Du må velge minst ett valg!") );
    }
}

} // namespace
} // namespace
