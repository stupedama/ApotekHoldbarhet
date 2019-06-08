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

#ifndef APOTEKH_PRINT_DURABILITY_H
#define APOTEKH_PRINT_DURABILITY_H

#include <QDialog>
#include <QMessageBox>
#include <QtPrintSupport/QPrinter>
#include <QtPrintSupport/QPrintDialog>
#include <QString>
#include <QTextDocument>
#include <QTextStream>
#include "../product.h"
#include "../constants.h"
#include "../calculate_months.h"
#include "../container.h"

namespace Ui {
class Print_durability;
}

namespace apotek {
namespace apotekholdbarhet {

class Print_durability : public QDialog
{
    Q_OBJECT

public:
    explicit Print_durability(const ProductsContainer& products, QWidget *parent = nullptr);
    ~Print_durability();
    void print() const;
    void add_color_red();
    void add_color_orange();
    void add_color_yellow();
    void add_color_white();
    QString make_top_html() const;
    QString make_title_html() const;
    QString make_table_html() const;
    QString make_bottom_html() const;
    QString make_html() const;

private slots:
    void on_buttonBox_accepted();

private:
    Ui::Print_durability *ui;
    ProductsContainer m_products;
    ProductsContainer m_print_products;
};

} // namespace
} // namespace

#endif // PRINT_DURABILITY_H
