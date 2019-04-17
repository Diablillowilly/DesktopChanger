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

class Backend : public QObject
{
    Q_OBJECT
public:
    explicit Backend(QObject *parent = nullptr);

    QNetworkAccessManager download_manager;
    QNetworkReply *currentDownload;


    void doDownload(const QUrl &url);
    bool saveToDisk(const QString &filename, QIODevice *data);
    static bool isHttpRedirect(QNetworkReply *reply);


private:

    httpRequest getWeb;

    QString filePath;


    void download(QString url);
    const QString myURL = "https://www.maerklin.de/de/service/multimedia/hintergrundbilder/hintergrundbilder";
    const QString myDURL = "https://www.maerklin.de";

signals:
    void finished();
public slots:
    void run();
    void run2();
    void run3();
    void close();


    void downloadFinished(QNetworkReply *reply);

private slots:
    void getReqFinished(QString answer);



};

#endif // BACKEND_HPP
