#include "backend.hpp"

#define ENABLE_CHANGE_BACKGROUND

Backend::Backend(QObject *parent) : QObject(parent)
{
    tempFolderPath = QDir::tempPath() + '/' + tempFolder;

    filePath = tempFolderPath;
    fullFilePath = filePath + '/' + fileName;
    qDebug()<<fileName;
    qDebug()<<filePath;
    qDebug()<<fullFilePath;
    qDebug()<<tempFolderPath;

    QObject::connect(&getWeb, SIGNAL(finished(QString)),
                     this, SLOT(getReqFinished(QString)));

    QObject::connect(&getFile, SIGNAL(finished()),
                     this, SLOT(fileDownloadFinished()));

}

void Backend::start(){
    qDebug() << "Starting";
    QDir myTempDir;

    if(myTempDir.exists(tempFolderPath)){
        qDebug()<<"temporal directory already exists";
    }else{
        qDebug()<<"temporal directory does not exist, creating directory";
        if(myTempDir.mkdir(tempFolderPath)){
            qDebug()<<"temporal directory created (" << tempFolderPath << ')';
        }else{
            qCritical()<<"Coudln't create directory: " << tempFolderPath;
        }
    }
    if(!getWeb.httpGet(QUrl(myURL)))
        qDebug()<<"URL has something wring (void Backend::start())";
    qDebug()<<"GET request done";


}

QUrl Backend::getDURL(QString webPage){

    QStringList list = webPage.split("\n", QString::SkipEmptyParts );

    for (int i = 0; i < list.size(); ++i){
        QString line = list.at(i).toLocal8Bit().constData();
        if(line.contains("1920 x 1080", Qt::CaseInsensitive)){
            QString DURL = myURL.scheme() + "://" + myURL.host();

            QStringList subList = line.split('"');
            for(QString str : subList){
                if(str.contains(".jpg")){
                    DURL+=str;
                    qDebug() <<"Download URL: "<< DURL;
                    if(!QUrl(DURL).isValid()){
                        qCritical() << "Download URL is not valid";
                    }else{
                        return QUrl(DURL);
                    }
                }
            }
        }
    }
    qFatal("ERROR (Backend::getDURL(QString webPage))");
}
bool Backend::checkDURL(QString webPage){

    QStringList list = webPage.split("\n", QString::SkipEmptyParts );

    for (int i = 0; i < list.size(); ++i){
        QString line = list.at(i).toLocal8Bit().constData();
        if(line.contains("1920 x 1080", Qt::CaseInsensitive)){
            QString DURL = myURL.scheme() + "://" + myURL.host();

            QStringList subList = line.split('"');
            for(QString str : subList){
                if(str.contains(".jpg")){
                    return true;//gota check it
                }
            }
        }
    }
    return false;
}


void Backend::getReqFinished(QString answer){
    if(checkDURL(answer)){

        QUrl myDUrl = getDURL(answer);
        getFile.download(myDUrl,fileName,filePath);
    }else{
        qCritical() << "Could not find any download link";
        emit close();
    }


}
void Backend::fileDownloadFinished(){
    if(!setBackground()){
        qCritical()<<"could not set background";
    }else{
        qDebug()<<"correctly set background";
    }
    emit close();
}

bool Backend::setBackground(){
    qDebug() << "Background path: " << fullFilePath;

#ifdef ENABLE_CHANGE_BACKGROUND
   //attempt to remove the cast warning
   // std::string foo = "var";
   // PVOID new_cast = (PVOID)foo.c_str();
   // PVOID old_cast = static_cast<PVOID>(foo.c_str());


    if(SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, (PVOID)fullFilePath.toStdString().c_str(), SPIF_SENDWININICHANGE | SPIF_UPDATEINIFILE))
        return true;

#endif

    return false;
}

