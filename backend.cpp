#include "backend.hpp"

#define ENABLE_CHANGE_BACKGROUND

Backend::Backend(QObject *parent) : QObject(parent)
{
    tempFolderPath = QDir::tempPath() + '/' + tempFolder;

    filePath = tempFolderPath;
    fullFilePath = filePath + '/' + fileName;
    storedDateFilePath = QStandardPaths::writableLocation(QStandardPaths::GenericConfigLocation) + '/' + tempFolder;

    storedDateFileFullPath = QStandardPaths::writableLocation(QStandardPaths::GenericConfigLocation) + '/' + tempFolder + '/' + storedDateFileName;

    QObject::connect(&getWeb, SIGNAL(finished(QString)),
                     this, SLOT(getReqFinished(QString)));

    QObject::connect(&getFile, SIGNAL(finished()),
                     this, SLOT(fileDownloadFinished()));

}

void Backend::start(){
    qDebug() << "Starting";

    int currentMonth = QDate::currentDate().month();

    //check for config dir
    QDir myConfigDir;
    if(myConfigDir.exists(storedDateFilePath)){
        qDebug()<<"config directory already exists";
    }else{
        qDebug()<<"config directory does not exist, creating directory";
        if(myConfigDir.mkdir(storedDateFilePath)){
            qDebug()<<"config directory created (" << storedDateFilePath << ')';
        }else{
            qCritical()<<"Coudln't create directory: " << storedDateFilePath;
        }
    }

    QFile file(storedDateFileFullPath);

    if(!file.open(QIODevice::ReadWrite)) {
        qDebug() << "Could not open "<< storedDateFileFullPath;


    }else{

        QTextStream in(&file);
        QString lastStoredDate;
        while(!in.atEnd()) {
            lastStoredDate = in.readAll();
        }
        file.close();

        if(lastStoredDate.toInt() != currentMonth){
            qDebug()<<"Last stored month is different from current month; "
                   << "updating background (Last stored month: "<< lastStoredDate << ")";



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

            if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
                qDebug() << "Could not open "<< storedDateFileFullPath;

            }else{
                QTextStream out(&file);
                out << currentMonth;
                file.close();
            }
        }else{
            qDebug()<<"Last stored month is same as current month; "
                   << "(Last stored month: "<< lastStoredDate << ")";
            emit close();
        }
    }

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
    //std::string foo = "var";
    //PVOID old_cast = (PVOID)foo.c_str();
    //PVOID new_cast = const_cast<void*>(reinterpret_cast<const void*>(fullFilePath.toStdString().data()));
    //PVOID new_cast_ = const_cast<void*>(fullFilePath.toStdString().data());
    //char *test =  const_cast<char *>(foo.data());
    //PVOID new_cast_ = reinterpret_cast<void *>(test);
    //void* a = new_cast;
    //PVOID fullFilePathCasted = const_cast<void*>(reinterpret_cast<const void*>(fullFilePath.toStdString().c_str()));
    //PVOID fullFilePathCasted = fullFilePath.toStdString().data();
    //int return_value = SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, (void*)"C:/Users/Willy/AppData/Local/Temp/Desktop Changer/background.jpg", SPIF_UPDATEINIFILE);
    //qDebug() <<SystemParametersInfo(SPI_SETDESKWALLPAPER, 0,(PVOID)"F:/Imagenes/121357.jpg", SPIF_SENDWININICHANGE | SPIF_UPDATEINIFILE);

    //if(SystemParametersInfo(SPI_SETDESKWALLPAPER, 0,(PVOID)"F:/Imagenes/121357.jpg", SPIF_SENDWININICHANGE | SPIF_UPDATEINIFILE))
       // return true;
   /* std::string fullFilePath = "F:/Imagenes/rwby_black_wallpaper.jpg";
    PVOID fullFilePathCasted_ = const_cast<void*>(reinterpret_cast<const void*>(fullFilePath.c_str()));
    LPWSTR test = L"F:/Imagenes/121357.jpg";

    int result = SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, test, SPIF_UPDATEINIFILE);
    SystemParametersInfo(SPI_SETDESKWALLPAPER, 0,fullFilePathCasted_,*//* SPIF_SENDWININICHANGE |*//* SPIF_UPDATEINIFILE);

    */
    //bool ret = SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, fullFilePathCasted, SPIF_SENDWININICHANGE | SPIF_UPDATEINIFILE);

    PVOID fullFilePathCasted = const_cast<void*>(reinterpret_cast<const void*>(fullFilePath.utf16()));
       if(SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, fullFilePathCasted, SPIF_SENDWININICHANGE | SPIF_UPDATEINIFILE))
            return true;
#endif

    return false;
}
