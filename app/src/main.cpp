#include "ui/apotekholdbarhet.h"
#include <QApplication>
#include <QSplashScreen>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    QPixmap pixmap(":images/images/loadingsceen.gif");

    QSplashScreen* splash = new QSplashScreen(pixmap);
    splash->show();

    const QColor color = QColor::fromRgb(255, 255, 255);

    splash->showMessage("Katten laster inn databasen. Vær litt tålmodig!", Qt::AlignCenter, color);

    a.processEvents();

    apotek::apotekholdbarhet::ApotekHoldbarhet w;

    w.show();
    splash->finish(&w);

    return a.exec();

}
