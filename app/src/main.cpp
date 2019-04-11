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

#include "ui/apotekholdbarhet.h"
#include <QApplication>
#include <QSplashScreen>
#include "download_manager.h"

int main(int argc, char *argv[])
{

    using namespace apotek::apotekholdbarhet;

    QApplication a(argc, argv);
    QPixmap pixmap(":images/images/loadingscreen.png");

    QSplashScreen* splash = new QSplashScreen(pixmap);
    splash->show();

    // download ean_codes.xml from the ApotekHoldbarhet repo.
    DownloadManager manager;
    QTimer::singleShot(0, &manager, SLOT(execute()));

    a.processEvents();

    ApotekHoldbarhet w;

    w.show();
    splash->finish(&w);

    return a.exec();
}
