#include "main.h"


myprogram::myprogram(QObject *parent) : QObject(parent)
{
    qDebug()<<"Openssl"<< QSslSocket :: supportsSsl () << QSslSocket::sslLibraryBuildVersionString();

    TickTimer = new QTimer(this);
    connect(TickTimer, SIGNAL(timeout()), this, SLOT(TimerTick()));
    TickTimer->start(1000);

    // TCP client
//    TurnSock = new QTcpSocket(this);
//    connect(TurnSock, SIGNAL(readyRead()), this, SLOT(TurnSokReadyRead()));
//    connect(TurnSock, SIGNAL(connected()), this, SLOT(TurnSokConnected()));
//    connect(TurnSock, SIGNAL(disconnected()), this, SLOT(TurnSokDisconnected()));
//    TurnSock->connectToHost("192.168.233.96", 80);


    // tcp server
    tcpserver = new QTcpServer(this);
    tcpserver->listen(QHostAddress::Any, 8080);
    connect(tcpserver, &QTcpServer::newConnection, this, &myprogram::TcpConnectedToClient);
}
//////////////////////////////////////////////////////////////
void myprogram::ReadInformation()
{
    QByteArray temp = tcpsocket->readAll();
//    qDebug() << "Message:" << temp;
    SendPayme(temp);
}
void myprogram::TcpConnectedToClient()
{
    qDebug()<<"NEW tcp connection";

    tcpsocket = tcpserver->nextPendingConnection();
    QString ip = tcpsocket->peerAddress().toString();
//    quint16 port = tcpsocket->peerPort();
//    ui->textEditRead->setText(QString("[%1:%2]连接成功").arg(ip).arg(port));
    connect(tcpsocket, &QTcpSocket::readyRead, this, &myprogram::ReadInformation);
}
/////////////////////////////////////////////////////////////
void myprogram::SendPayme(QByteArray qrData)
{
    qDebug() << qrData;

    // Body QJson
    QJsonObject json;

    // request_dt polya
    QDate cDate = QDate::currentDate();
    QTime cTime = QTime::currentTime();

    QString DTime = QString("%1 %2")
                        .arg(cDate.toString("yyyy-MM-dd"))
                        .arg(cTime.toString("hh:mm:ss"));

    // json.insert("request_dt", "2019-02-21 16:10:23");
    // json.insert("last_tid", 0);
    // json.insert("limit", 50);

    json.insert("hid", 23);
    json.insert("billing_code", qrData.data());
    json.insert("request_dt", DTime);

    QJsonDocument jsondoc(json);
    QByteArray JsonData = jsondoc.toJson();

    // QNetwork
    QNetworkAccessManager * manager = new QNetworkAccessManager(this);
    QUrl url("https://service.khivamuseum.uz/api/turnstile/ticket/new");
    // QUrl url("https://service.khivamuseum.uz/api/turnstile/ticket/list");
    // QUrl url("http://192.168.233.96");
    QNetworkRequest request(url);

    // Headers
    QString HeaderData = "Basic dF9ocy1rM3hqMWk3Y01XbTU6WmRjOWFMaU9ae1ZqZSQxU2k1MHhuR2Rl";

    request.setRawHeader("Authorization", HeaderData.toLocal8Bit());
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setHeader(QNetworkRequest::ContentLengthHeader, QByteArray::number(JsonData.size()));

    connect(manager, SIGNAL(finished(QNetworkReply *)), this, SLOT(replyFinished(QNetworkReply *)));
    manager->post(request, JsonData);
}

void myprogram::replyFinished(QNetworkReply *reply)
{
    QByteArray data = reply->readAll();

    QString str(data);
    qDebug() << str;

    QFile f("site.html");
    if (f.open(QFile::WriteOnly))
        f.write(data);

    tcpsocket->write(data);
}
////////////////////////////////////////////////////////////////
void myprogram::TimerTick()
{
    qDebug()<<"Tick";
}
////////////////////////////////////////////////////////////////
// TCP client Slots
void myprogram::TurnSokReadyRead()
{
    qDebug()<<"Ready Read";
    TurnSock->write("some buffer");
}

void myprogram::TurnSokConnected()
{
    qDebug()<<"Connected";
}

void myprogram::TurnSokDisconnected()
{
    qDebug()<<"DisConnected";
}
