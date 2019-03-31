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

#include "about_apotekholdbarhet.h"
#include "ui_about_apotekholdbarhet.h"

namespace apotek {
namespace apotekholdbarhet {

AboutApotekHoldbarhet::AboutApotekHoldbarhet(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutApotekHoldbarhet)
{
    ui->setupUi(this);
    ui->textBrowser->setOpenExternalLinks(true);
    ui->textBrowser_2->setOpenExternalLinks(true);

    QString title = ui->label_title->text();
    ui->label_title->setText(title + " " + apotek::apotekholdbarhet::apotek_program_version);

    compiled_info();
}

AboutApotekHoldbarhet::~AboutApotekHoldbarhet()
{
    delete ui;
}

void AboutApotekHoldbarhet::compiled_info() const
{
    // save the text already in the label ui
    const QString saved_label_compiled_info = ui->label_compiled_info->text();
    const QString saved_label_qt_version = ui->label_qt_version->text();
    const QString saved_label_date = ui->label_compiled_date->text();

    // get todays date
    const QString todays_date = get_todays_date().toString(apotek::constants::date_format);

    // generate rest of the string
    const QString label_compiled = saved_label_compiled_info + " " + get_compiler_version();
    const QString label_qtversion = saved_label_qt_version + get_qt_version();
    const QString label_date = saved_label_date + todays_date;

    // set the ui
    ui->label_compiled_info->setText(label_compiled);
    ui->label_qt_version->setText(label_qtversion);
    ui->label_compiled_date->setText(label_date);
}

QDate AboutApotekHoldbarhet::get_todays_date() const
{
    using namespace date;
    const sys_days today = floor<days>(std::chrono::system_clock::now());
    return QDate::fromJulianDay((today.time_since_epoch() +
                                 (sys_days{1970_y/jan/1} - sys_days{year{-4713}/nov/24})).count());

}

QString AboutApotekHoldbarhet::get_compiler_version() const
{

#ifdef __clang__
    return "clang version " + QString::fromStdString(__clang_version__);
#elif __GNUC__
    return "g++ (GCC) " + QString::fromStdString(__VERSION__);
#elif _MSC_FULL_VER
    return "Msvc++ " + QString::number(_MSC_FULL_VER);
#elif __MINGW32__
    return "mingw32 " + QString::number(__MINGW32_MAJOR_VERSION) + "." + QString::number(__MINGW32_MINOR_VERSION);
#else
    return "ukjent compiler ";
#endif
}

QString AboutApotekHoldbarhet::get_qt_version() const
{
#ifdef QT_VERSION
    QString version;
    version = QString::number(QT_VERSION_MAJOR) + "." + QString::number(QT_VERSION_MINOR) + " (" + QString::number(QT_VERSION) + ")";
    return version;
#else
    return "ukjent.";
#endif
}

} // namespace
} // namespace
