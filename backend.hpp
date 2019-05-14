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
    QString tempFolderPath;

    const QString fileName = "background.jpg";
    QString filePath = "C:";
    QString fullFilePath;

    const QString storedDateFileName = "lastDate.txt";
    QString storedDateFilePath;
    QString storedDateFileFullPath;

    httpRequest getWeb;
    fileDownloader getFile;
    parser myWebparse;

    const QString germanMonths[12] = {"Januar", "Februar", "März", "April", "Mai", "Juni", "Juli", "August", "September", "Oktober", "November", "Dezember"};

    //this is the date as found in the marklin webpage: "monthInGerman year"
    QString currentDateMarklinFormat;

    QUrl getDURL(QString webPage);
    bool checkDURL(QString webPage);

    bool setBackground();

    QString getLastStoredDate();
    bool setStoredDate(QString newDate);
    bool checkForFile();
    bool createPath();



signals:
    void close();
public slots:
    void start();

private slots:
    void getReqFinished(QString answer);
    void fileDownloadFinished();



};

#endif // BACKEND_HPP
