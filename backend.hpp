#ifndef BACKEND_HPP
#define BACKEND_HPP

#include <QObject>
#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QTextStream>
#include <QFile>
#include <QDir>

#include <windows.h>

#include <iostream> //cout/cin etc
#include <string.h> //pra strings

#include "httprequest.hpp"
#include "filedownloader.hpp"

#include <QStandardPaths>


class Backend : public QObject
{
    Q_OBJECT
public:
    explicit Backend(QObject *parent = nullptr);



private:

    QUrl myURL = QUrl("https://www.maerklin.de/de/service/multimedia/hintergrundbilder/hintergrundbilder");
    QString tempFolder = "Desktop Changer";
    QString tempFolderPath;

    QString fileName = "filename.jpg";
    QString filePath = "C:";
    QString fullFilePath;


    httpRequest getWeb;
    fileDownloader getFile;


    QUrl getDURL(QString webPage);
    bool checkDURL(QString webPage);

    bool setBackground();



signals:
    void close();
public slots:
    void start();

private slots:
    void getReqFinished(QString answer);
    void fileDownloadFinished();



};

#endif // BACKEND_HPP
