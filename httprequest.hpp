#ifndef HTTPREQUEST_HPP
#define HTTPREQUEST_HPP

#include <QObject>
#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>

class httpRequest : public QObject
{
    Q_OBJECT

public:
    httpRequest(QObject *parent = nullptr);

    bool httpGet(QUrl url);
    QString getAnswer();

private:
    QNetworkAccessManager *manager;
    QNetworkRequest request;
    QString answer;


signals:
    void finished(QString answer);



public slots:

    void requestFinished(QNetworkReply *reply);

};

#endif // HTTPREQUEST_HPP
