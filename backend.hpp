#ifndef BACKEND_HPP
#define BACKEND_HPP

#include <QObject>
#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QTextStream>
#include <QFile>
#include <QDir>
#include <QDate>
#include <QDateTime>
#include <QStandardPaths>

#include <QSettings>

#include <windows.h>

#include <iostream> //cout/cin etc
#include <string.h> //pra strings

#include "httprequest.hpp"
#include "filedownloader.hpp"
#include "parser.hpp"



class Backend : public QObject
{
    Q_OBJECT
public:
    explicit Backend(QObject *parent = nullptr);



private:

    const QUrl myURL = QUrl("https://www.maerklin.de/de/service/multimedia/hintergrundbilder/hintergrundbilder");
    const QString tempFolder = "Desktop Changer";
    //QString tempFolderPath;

QString final_download_path;

    QString fileName = "background.jpg";

    //path to folder containing file
    QString filePath;

    //path to file
    QString fullFilePath;

    /*const QString storedDateFileName = "lastDate.txt";
    QString storedDateFilePath;
    QString storedDateFileFullPath;*/

    const QString storedConfig = "config.txt";
    //path to folder containing file
    QString storedConfigFilePath;
    //path to file
    QString storedConfigFileFullPath;

    static const int available_res_num = 6;
    const QString available_res[available_res_num] = {"1024 x 768","1280 x 1024","1280 x 800","1366 x 768","1680 x 1050", "1920 x 1080"};


    const QString res_key = "resolution";
    const QString res_def_value = "5";
    const QString store_backgrounds_key = "store_backgrounds";
    const QString store_backgrounds_def_value = "false";
    const QString store_backgrounds_path_key = "stored_backgrounds_path";
    const QString store_backgrounds_path_def_value = "";


    const QString last_date_key = "Cache/last_date";
    const QString last_res_key = "Cache/last_resolution";
    const QString last_store_backgrounds_key = "Cache/last_store_backgrounds";
    const QString last_store_backgrounds_path_key = "Cache/last_store_backgrounds_path";


    httpRequest getWeb;
    fileDownloader getFile;
    parser myWebparse;

    const QString germanMonths[12] = {"Januar", "Februar", "März", "April", "Mai", "Juni", "Juli", "August", "September", "Oktober", "November", "Dezember"};

    //this is the date as found in the marklin webpage: "monthInGerman year"
    QString currentDateMarklinFormat;

    QUrl getDURL(QString webPage);
    bool checkDURL(QString webPage);

    bool setBackground();

    /*
    QString getLastStoredDate();
    bool setStoredDate(QString newDate);
    bool checkForFile();
    bool createPath();*/



signals:
    void close();
public slots:
    void start();

private slots:
    void getReqFinished(QString answer);
    void fileDownloadFinished();



};

#endif // BACKEND_HPP
