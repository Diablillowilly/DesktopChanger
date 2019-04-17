#include "httprequest.hpp"

httpRequest::httpRequest(QObject *parent): QObject(parent)
{

    QObject::connect(&manager, SIGNAL(finished(QNetworkReply*)),
                     this, SLOT(requestFinished(QNetworkReply*)));

}


bool httpRequest::httpGet(QUrl url){
    if(!url.isValid())
        return false;

    request.setUrl(QUrl(url));
    manager.get(request);
    return true;
}


QString httpRequest::getAnswer(){
    return answer;
}






void httpRequest::requestFinished(QNetworkReply *reply) {
    if (reply->error()) {
        qCritical() << reply->errorString();
        return;
    }
    answer = reply->readAll();
    emit finished(answer);
}

