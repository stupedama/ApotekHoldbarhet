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

#ifndef APOTEKH_DOWNLOAD_H
#define APOTEKH_DOWNLOAD_H

#include <QtNetwork>
#include <QVector>
#include <QUrl>
#include <QString>
#include <QList>
#include <QSslError>
#include <QFile>
#include <QDebug>

/**
 * @brief The DownloadManager class
 * Handles downloading of files.
 * Currently do_download(...) downloads a xml file from the ApotekHoldbarhet
 * repo with ean_codes for the data matrixes.
 */
class DownloadManager : public QObject
{
    Q_OBJECT
public:
    DownloadManager();
    void do_download(const QUrl& url);
    static QString save_filename(const QUrl& url);
    bool save_to_disk(const QString& filename, QIODevice *data);
    static bool is_http_redirect(QNetworkReply* reply);
public slots:
    void execute();
    void download_finished(QNetworkReply* reply);
    void sslErrors(const QList<QSslError> &sslErrors); // CamelCase because its overloaded function
private:
    QNetworkAccessManager manager;
    QVector<QNetworkReply*> current_downloads;
};

#endif // DOWNLOAD_H
