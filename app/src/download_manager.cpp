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

#include "download_manager.h"

DownloadManager::DownloadManager()
{
    connect(&manager, SIGNAL(finished(QNetworkReply*)),
            SLOT(download_finished(QNetworkReply*)));
}

void DownloadManager::sslErrors(const QList<QSslError> &sslErrors)
{
#if QT_CONFIG(ssl)
    for (const QSslError &error : sslErrors)
        fprintf(stderr, "SSL error: %s\n", qPrintable(error.errorString()));
#else
    Q_UNUSED(sslErrors);
#endif
}

void DownloadManager::do_download(const QUrl &url)
{
    QNetworkRequest request(url);
    QNetworkReply *reply = manager.get(request);

#if QT_CONFIG(ssl)
    connect(reply, SIGNAL(sslErrors(QList<QSslError>)),
            SLOT(sslErrors(QList<QSslError>)));
#endif

    current_downloads.append(reply);
}

QString DownloadManager::save_filename(const QUrl &url)
{
    QString basename;
    QString path = url.path();
    if(path.startsWith("https://raw")) {
        basename = "fmd/" + QFileInfo(path).fileName();
    } else {
        basename = QFileInfo(path).fileName();
    }

    if (basename.isEmpty())
        basename = "download";

    return basename;
}

bool DownloadManager::save_to_disk(const QString &filename, QIODevice *data)
{
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly)) {
        fprintf(stderr, "Could not open %s for writing: %s\n",
                qPrintable(filename),
                qPrintable(file.errorString()));
        return false;
    }

    file.write(data->readAll());
    file.close();

    return true;
}

bool DownloadManager::is_http_redirect(QNetworkReply *reply)
{
    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        return statusCode == 301 || statusCode == 302 || statusCode == 303
               || statusCode == 305 || statusCode == 307 || statusCode == 308;
}

void DownloadManager::execute()
{

    // download the ean_codes for the missing ean codes for data matrixes
    QUrl xml_url{"https://raw.githubusercontent.com/stupedama/ApotekHoldbarhet/master/app/xml/fmd/ean_codes.xml"};
    do_download(xml_url);

    // download current version
    // for future versions you can check if you are using the current version
    QUrl version_url{"http://cvf.no/apotek_version.ini"};
    do_download(version_url);
}

void DownloadManager::download_finished(QNetworkReply* reply)
{
    QUrl url = reply->url();
    if (reply->error()) {
        fprintf(stderr, "Download of %s failed: %s\n",
                url.toEncoded().constData(),
                qPrintable(reply->errorString()));
    } else {
        if (is_http_redirect(reply)) {
            fputs("Request was redirected.\n", stderr);
        } else {
            QString filename = save_filename(url);
            if (save_to_disk(filename, reply)) {
                printf("Download of %s succeeded (saved to %s)\n",
                       url.toEncoded().constData(), qPrintable(filename));
            }
        }
    }

    current_downloads.removeAll(reply);
    reply->deleteLater();
}
