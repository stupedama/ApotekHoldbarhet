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

#include "about_fest.h"
#include "ui_about_fest.h"
#include "date.h"

namespace apotek {
namespace apotekholdbarhet {

AboutFest::AboutFest(QString fest_hentetdato, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutFest),
    m_fest_hentetdato{fest_hentetdato}
{
    ui->setupUi(this);
    ui->textBrowser->setOpenExternalLinks(true);


    QString label_text_hentetdato = ui->fest_hentedato->text();
    QString new_label_text = label_text_hentetdato + " " + format_date(m_fest_hentetdato);

    // set the hentetdato
    ui->fest_hentedato->setText(new_label_text);
}

AboutFest::~AboutFest()
{
    delete ui;
}


// example of the xml timeformat is 2019-02-08T13:08:02
// get all the string until we hit T.
QString AboutFest::format_date(QString d) const
{
    QString formatted_date;

    for(auto n{0}; n < d.length(); ++n) {
        if(d.at(n) != 'T') {
            formatted_date += d.at(n);
        } else {
            formatted_date += " - ";
        }
    }

    return formatted_date;
}

} // namespace
} // namespace
