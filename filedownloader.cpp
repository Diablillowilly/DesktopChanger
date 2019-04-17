#include "filedownloader.hpp"

fileDownloader::fileDownloader(QObject *parent) : QObject(parent)
{
    QObject::connect(&download_manager, SIGNAL(finished(QNetworkReply*)),
                     SLOT(downloadFinished(QNetworkReply*)));

}



void fileDownloader::download(QUrl DURL,QString newFileName, QString newFilePath){
    filePath=newFilePath;
    fileName=newFileName;
    fullFilePath = filePath + '/' + fileName;
    QNetworkRequest request(DURL);
    currentDownload = download_manager.get(request);
    qDebug() <<"Downloading: " << DURL.fileName();
    return;
}

/*
bool fileDownloader::isHttpRedirect(QNetworkReply *reply)
{
    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    return statusCode == 301 || statusCode == 302 || statusCode == 303
            || statusCode == 305 || statusCode == 307 || statusCode == 308;
}*/


void fileDownloader::downloadFinished(QNetworkReply *reply)
{
    QUrl url = reply->url();

    if (reply->error()) {
        qCritical() <<  "Download of " <<url.toEncoded().constData() <<" failed:"
                     << reply->errorString();
    } else {
        /* if (isHttpRedirect(reply)) {
            fputs("Request was redirected.\n", stderr);
        } else {*/

        if (saveToDisk(reply)) {
            qDebug() << "Download of " << url.fileName()
                     << " succeeded (saved to" << fullFilePath << ")";

        }
        //}
    }

    currentDownload->deleteLater();
    reply->deleteLater();



    emit finished();
}

bool fileDownloader::saveToDisk(QIODevice *data)
{
    QFile file(fullFilePath);
    if (!file.open(QIODevice::WriteOnly)) {
        qCritical() << "Could not open "<< fileName <<
                       " for writing: " << file.errorString();
        return false;
    }

    file.write(data->readAll());

    file.close();

    return true;
}
