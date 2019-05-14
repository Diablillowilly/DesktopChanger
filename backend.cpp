#include "backend.hpp"

#define ENABLE_CHANGE_BACKGROUND

Backend::Backend(QObject *parent) : QObject(parent)
{
    //this is the date as found in the marklin webpage: "monthInGerman year"
    currentDateMarklinFormat = germanMonths[QDate::currentDate().month() - 1] + ' ' + QString::number(QDate::currentDate().year());


    //storedDateFileFullPath = QStandardPaths::writableLocation(QStandardPaths::GenericConfigLocation) + '/' + tempFolder + '/' + storedDateFileName;

    storedConfigFilePath = QStandardPaths::writableLocation(QStandardPaths::GenericConfigLocation) + '/' + tempFolder;
    storedConfigFileFullPath = QStandardPaths::writableLocation(QStandardPaths::GenericConfigLocation) + '/' + tempFolder + '/' + storedConfig;


    //initialize settings and assign default values
    QSettings settings(storedConfigFileFullPath, QSettings::IniFormat);


    if(!settings.contains(res_key)){
        settings.setValue(res_key, res_def_value);
    }
    if(!settings.contains(store_backgrounds_key)){
        settings.setValue(store_backgrounds_key, store_backgrounds_def_value);
    }
    if(!settings.contains(store_backgrounds_path_key)){
        settings.setValue(store_backgrounds_path_key, store_backgrounds_path_def_value);
    }
    //settings.sync();
    //qDebug() << settings.fileName();



    QString tempFolderPath = QDir::tempPath() + '/' + tempFolder;

    filePath = tempFolderPath;
    fileName = QString::number(QDate::currentDate().year()) + ' ' + germanMonths[QDate::currentDate().month() - 1] + ' ' + available_res[settings.value(res_key).toInt()] + ".jpg";
    fullFilePath = filePath + '/' + fileName;
    //storedDateFilePath = QStandardPaths::writableLocation(QStandardPaths::GenericConfigLocation) + '/' + tempFolder;






    QObject::connect(&getWeb, SIGNAL(finished(QString)),
                     this, SLOT(getReqFinished(QString)));

    QObject::connect(&getFile, SIGNAL(finished()),
                     this, SLOT(fileDownloadFinished()));



}

void Backend::start(){
    QSettings settings(storedConfigFileFullPath, QSettings::IniFormat);
    qDebug() << "Starting";




    //check for config dir
    /* if(!createPath()){
        emit close();
    }*/



    if(settings.value(last_date_key).toString() == currentDateMarklinFormat && settings.value(last_res_key).toString() == settings.value(res_key).toString() &&  settings.value(last_store_backgrounds_key).toString() == settings.value(store_backgrounds_key).toString() &&  settings.value(store_backgrounds_path_key).toString() == settings.value(last_store_backgrounds_path_key).toString()){
        qDebug()<<"Last stored data is the same as current data; "
               << "(Last stored month: "<< settings.value(last_date_key).toString() << ")"
               << "(Last stored res: "<< settings.value(last_res_key).toString() << ")"
               << "(Last stored res: "<< settings.value(last_store_backgrounds_key).toString() << ")"
               << "(Last stored res: "<< settings.value(last_store_backgrounds_path_key).toString() << ")";
        emit close();
        return;
    }
    /*
    qDebug()<<settings.value(last_date_key).toString();
    qDebug()<<currentDateMarklinFormat;
    qDebug()<<settings.value(last_res_key).toString();
    qDebug()<<settings.value(res_key).toString();
    qDebug()<<settings.value(last_store_backgrounds_key).toString();
    qDebug()<<settings.value(store_backgrounds_key).toString();
    qDebug()<<"------------------------------------------------";
    bool test = settings.value(last_date_key).toString() == currentDateMarklinFormat;
    qDebug()<< test;
    test = settings.value(last_res_key).toString() == settings.value(res_key).toString();
    qDebug()<< test;
    test = settings.value(last_store_backgrounds_key).toString() == settings.value(store_backgrounds_key).toString();
    qDebug()<< test;
    */

    qDebug()<< "Stored date or stored res, which only changes on successfull desktop change, is different from the current date or res";




    if(!getWeb.httpGet(QUrl(myURL)))
        qDebug()<<"URL has something wrong (void Backend::start())";

    qDebug()<<"GET request done";


    /*

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
*/



}


/*
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
}*/



void Backend::getReqFinished(QString answer){
    QSettings settings(storedConfigFileFullPath, QSettings::IniFormat);
    myWebparse.setWebPage(answer);
    if(!myWebparse.parse_web_page()){
        qCritical() << "error parsing webpage";
        emit close();
    }

    QString Base_URL = myURL.scheme() + "://" + myURL.host();
    QUrl myDUrl = Base_URL + myWebparse.getDateResURL(0, settings.value(res_key).toInt());



    //if the current date or res is different from the last retrieved from the website,
    //it means that the website hasnt updated yet the background, or that they have posted it too soon,
    //anyway, there is nothing to do
    if(myWebparse.getDate() == currentDateMarklinFormat){

        qDebug() << "Stored and current date is the same as the webpage date; starting http req";

        if(settings.value(store_backgrounds_key).toString() == "true"){
            // if(settings.value(store_backgrounds_path_key).toString().)
            final_download_path = settings.value(store_backgrounds_path_key).toString();
            qDebug() << "true " << final_download_path;
        }else if (settings.value(store_backgrounds_key).toString() == "false"){
            final_download_path = filePath;
        }else{
            qCritical() << "settings are wrong, store_backgrounds_key, is not neither true nor false (void Backend::getReqFinished(QString answer))";
            emit close();
        }
        getFile.download(myDUrl,fileName,final_download_path);
    }else{
        qDebug() << "Stored and current date is different from the webpage date";
        emit close();
    }
}

void Backend::fileDownloadFinished(){
    QSettings settings(storedConfigFileFullPath, QSettings::IniFormat);
    if(!setBackground()){
        qCritical()<<"could not set background";
    }else{
        qDebug()<<"correctly set background";

        qDebug()<<"Updating stored date";
        settings.setValue(last_date_key, currentDateMarklinFormat);
        settings.setValue(last_res_key, settings.value(res_key).toString());
        settings.setValue(last_store_backgrounds_key, settings.value(store_backgrounds_key).toString());
        settings.setValue(last_store_backgrounds_path_key, settings.value(store_backgrounds_path_key).toString());
    }
    emit close();
}

bool Backend::setBackground(){

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


    QString full_final_download_path = final_download_path + '/' + fileName;
    qDebug() << "Final download path: " << full_final_download_path;

    PVOID fullFilePathCasted = const_cast<void*>(reinterpret_cast<const void*>(full_final_download_path.utf16()));
    if(SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, fullFilePathCasted, SPIF_SENDWININICHANGE | SPIF_UPDATEINIFILE))
        return true;
#endif

    return false;
}

/*
QString Backend::getLastStoredDate(){
    QFile file(storedDateFileFullPath);

    QString lastStoredDate;

    if(!file.open(QIODevice::ReadWrite)) {
        qDebug() << "Could not open "<< storedDateFileFullPath;


    }else{

        QTextStream in(&file);
        while(!in.atEnd()) {
            lastStoredDate = in.readAll();
        }
        file.close();
    }
    return lastStoredDate;
}

bool Backend::setStoredDate(QString newDate){
    QFile file(storedDateFileFullPath);

    if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        qCritical() << "Could not open "<< storedDateFileFullPath;
        return false;
    }else{
        QTextStream out(&file);
        out << newDate;
        file.close();
    }
    return true;
}

bool Backend::checkForFile(){
    QFile file(storedDateFileFullPath);
    return file.exists();
}

bool Backend::createPath(){

    QDir myConfigDir;
    if(myConfigDir.exists(storedDateFilePath)){
        qDebug()<<"config directory already exists";
    }else{
        qDebug()<<"config directory does not exist, creating directory";
        if(myConfigDir.mkdir(storedDateFilePath)){
            qDebug()<<"config directory created (" << storedDateFilePath << ')';
        }else{
            qCritical()<<"Coudln't create directory: " << storedDateFilePath;
            return false;
        }
    }

    return true;
}
*/
