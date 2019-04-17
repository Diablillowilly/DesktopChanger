#ifndef FILEDOWNLOADER_HPP
#define FILEDOWNLOADER_HPP

#include <QObject>
#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QFile>
#include <QDir>

class fileDownloader : public QObject
{
    Q_OBJECT
public:
    explicit fileDownloader(QObject *parent = nullptr);


    QNetworkAccessManager download_manager;
    QNetworkReply *currentDownload;


    void download(QUrl url,QString fileName, QString filePath);
    bool saveToDisk(QIODevice *data);
    //static bool isHttpRedirect(QNetworkReply *reply);

private:

    QString filePath;
    QString fileName;
    QString fullFilePath;

    const QString myDURL = "https://www.maerklin.de";

signals:
void finished();

public slots:
    void downloadFinished(QNetworkReply *reply);

};

#endif // FILEDOWNLOADER_HPP
