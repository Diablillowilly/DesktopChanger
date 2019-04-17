#include "backend.hpp"

Backend::Backend(QObject *parent) : QObject(parent)
{
    qDebug("wwww");
    manager = new QNetworkAccessManager();
    QObject::connect(manager, &QNetworkAccessManager::finished,
                     this, [=](QNetworkReply *reply) {
        if (reply->error()) {
            qDebug() << reply->errorString();
            return;
        }

        answer = reply->readAll();

        //qDebug() << "!"<<answer;

    }
    );

    QObject::connect(manager, SIGNAL(finished(QNetworkReply*)),
                     this, SLOT(managerFinished(QNetworkReply*)));

    QObject::connect(&download_manager, SIGNAL(finished(QNetworkReply*)),
            SLOT(downloadFinished(QNetworkReply*)));

}
void Backend::run(){
    qDebug("run");




}
void Backend::run2(){

    request.setUrl(QUrl(myURL));
    manager->get(request);


}
void Backend::run3(){
    //qDebug() << answer;
    qDebug() << answer.length();

    QStringList list1 = answer.split("\n", QString::SkipEmptyParts );
    qDebug()<<"lines->" <<list1.size();
    for (int i = 0; i < list1.size(); ++i){
        // qDebug() << list1.at(i).toLocal8Bit().constData();
        QString line = list1.at(i).toLocal8Bit().constData();
        if(line.contains("1920 x 1080", Qt::CaseInsensitive)){
            QString DURL = getDUrl(line);
            qDebug() <<"-------------"<< DURL;

            download(DURL);
        QString file_path = QDir::currentPath() + "/download.jpg";
        qDebug() <<"--------"<< file_path<<"---------";
filePath = file_path;
            return;
        }


    }




    QString filename="Data.txt";
    QFile file( filename );
    if ( file.open(QIODevice::ReadWrite) )
    {
        QTextStream stream( &file );
        stream <<"<<<" <<answer<<">>>" << endl;
    }

    file.close();

}
void Backend::close(){

    emit finished();


}
void Backend::managerFinished(QNetworkReply *reply) {
    if (reply->error()) {
        qDebug() << reply->errorString();
        return;
    }

    QString answer = reply->readAll();

    qDebug() << answer;
}


QString Backend::getDUrl(QString url){
    QString response = myDURL;

    QStringList list1 = url.split('"');
    for(QString str : list1){
        if(str.contains(".jpg")){
            response+=str;
            //qDebug() << response;
        }
    }

    return response;

}
void Backend::download(QString DURL){

qDebug() <<"Downloading:" <<DURL;
    QNetworkRequest request(DURL);
     currentDownload = download_manager.get(request);



    return;

}
bool Backend::isHttpRedirect(QNetworkReply *reply)
{
    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    return statusCode == 301 || statusCode == 302 || statusCode == 303
           || statusCode == 305 || statusCode == 307 || statusCode == 308;
}


void Backend::downloadFinished(QNetworkReply *reply)
{
    QUrl url = reply->url();
    if (reply->error()) {
        fprintf(stderr, "Download of %s failed: %s\n",
                url.toEncoded().constData(),
                qPrintable(reply->errorString()));
    } else {
        if (isHttpRedirect(reply)) {
            fputs("Request was redirected.\n", stderr);
        } else {
            QString filename = "download.jpg";
            if (saveToDisk(filename, reply)) {
                printf("Download of %s succeeded (saved to %s)\n",
                       url.toEncoded().constData(), qPrintable(filename));
            }
        }
    }

    currentDownload->deleteLater();
    reply->deleteLater();


    bool ret = SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, (PVOID)filePath.utf16(), SPIF_SENDWININICHANGE | SPIF_UPDATEINIFILE);


}

bool Backend::saveToDisk(const QString &filename, QIODevice *data)
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
