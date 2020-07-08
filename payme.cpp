#include "payme.h"

/// Constructor of class Payme
Payme::Payme(QObject *parent) : QObject(parent)
{

}
/////////////////////////////////////////////////////////
void Payme::create()
{
    qDebug() << "Payme creating started";

    // Body QJson
    QJsonObject json;
    QJsonObject jsonParams;

    jsonParams.insert("amount", 101 * 100);
    json.insert("id", "1105e3bab097f420a62ced0b");
    json.insert("method", "receipts.create");
    json.insert("params", jsonParams);

    QJsonDocument jsondoc(json);
    QByteArray JsonData = jsondoc.toJson();

    // QNetwork
    QNetworkAccessManager * manager = new QNetworkAccessManager(this);
    QUrl url("https://checkout.paycom.uz/api");

    // QUrl url("http://192.168.233.96");
    QNetworkRequest request(url);

    QString HeaderData = "5ef2dd55b18e52dd0c1af92a:&DJ%v5u6qbHSK8MWk580hdP%yfgqrE4zB84w";

    // Set Headers
    request.setHeader(QNetworkRequest::ContentLengthHeader, QByteArray::number(JsonData.size()));
    request.setRawHeader("Host", "checkout.paycom.uz");
    request.setRawHeader("Accept", "*/*");
    request.setRawHeader("Accept-Encoding", "gzip, deflate, br");
    request.setRawHeader("Connection", "keep-alive");
    request.setRawHeader("X-Auth", HeaderData.toLocal8Bit());
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    connect(manager, &QNetworkAccessManager::finished, this, &Payme::PaymeCreateRespond);
    manager->post(request, JsonData);
}

void Payme::PaymeCreateRespond(QNetworkReply *reply)
{
    QByteArray data = reply->readAll();

    QString str(data);
    qDebug() << str;

    QFile f("site.txt");
    if (f.open(QFile::WriteOnly))
        f.write(data);


//    tcpsocket->write(data);
//    QJsonDocument
}
/////////////////////////////////////////////////////////
void Payme::pay()
{

}

