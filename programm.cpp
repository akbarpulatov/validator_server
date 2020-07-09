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

    payme = new Payme();
    PaymeReceiptsCreate("100");

    // tcp server
    tcpserver = new QTcpServer(this);
    tcpserver->listen(QHostAddress::Any, 8080);
    connect(tcpserver, &QTcpServer::newConnection, this, &myprogram::TcpConnectedToClient);
}
//////////////////////////////////////////////////////////////
void myprogram::ReadInformation()
{
    QByteArray temp = tcpsocket->readAll();
    // qDebug() << "Message:" << temp;
    // SendPayme(temp);

    // payme craete function
    // payme->create();

    //receipt pay
    PaymeReceiptsPay(temp);
    tcpsocket->write("1");

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
void myprogram::PaymeReceiptsCreate(const QString amount)
{
    qDebug() << "Payme creating started";

    // Body QJson
    QJsonObject json;
    QJsonObject jsonParams;

    jsonParams.insert("amount", amount.toInt() * 100);
    json.insert("id", id);
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
    request.setRawHeader("Connection", "keep-alive");
    request.setRawHeader("X-Auth", HeaderData.toLocal8Bit());
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    connect(manager, SIGNAL(finished(QNetworkReply *)), this, SLOT(PaymeReceiptsCreateRespond(QNetworkReply *)));
    manager->post(request, JsonData);
}

void myprogram::PaymeReceiptsCreateRespond(QNetworkReply *reply)
{
    if (!reply->error())
    {
        QJsonDocument JDocResp = QJsonDocument::fromJson(reply->readAll());
        QJsonObject JObjResp = JDocResp.object();
        QJsonObject resultJson = JObjResp["result"].toObject();
        QJsonObject receiptJson = resultJson["receipt"].toObject();
        idRecipient = receiptJson["_id"].toString();
        qDebug() << "Payment is successfully created!";
    }
}
////////////////////////////////////////////////////////////////
void myprogram::PaymeReceiptsPay(const QByteArray token)
{
    qDebug() << "Payme Pay";

    // Body QJson
    QJsonObject json;
    QJsonObject jsonParams;

    jsonParams.insert("id", idRecipient);
    jsonParams.insert("token", token.data());

    json.insert("id", id);
    json.insert("method", "receipts.pay");
    json.insert("params", jsonParams);

    QJsonDocument jsondoc(json);
    QByteArray JsonData = jsondoc.toJson();

    // QNetwork
    QNetworkAccessManager * manager = new QNetworkAccessManager(this);
    QUrl url("https://checkout.paycom.uz/api");

    QNetworkRequest request(url);
    QString HeaderData = "5ef2dd55b18e52dd0c1af92a:&DJ%v5u6qbHSK8MWk580hdP%yfgqrE4zB84w";

    // Set Headers
    request.setHeader(QNetworkRequest::ContentLengthHeader, QByteArray::number(JsonData.size()));
    request.setRawHeader("Host", "checkout.paycom.uz");
    request.setRawHeader("Accept", "*/*");
    request.setRawHeader("Connection", "keep-alive");
    request.setRawHeader("X-Auth", HeaderData.toLocal8Bit());
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    connect(manager, SIGNAL(finished(QNetworkReply *)), this, SLOT(PaymeReceiptsPayRespond(QNetworkReply *)));
    manager->post(request, JsonData);
}

void myprogram::PaymeReceiptsPayRespond(QNetworkReply *reply)
{
    QByteArray data = reply->readAll();
    qDebug() << "Payme Pay responded" << endl << data << endl;

    // Payme create receipt
    PaymeReceiptsCreate("100");
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
