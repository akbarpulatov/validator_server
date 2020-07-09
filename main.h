#ifndef MAIN_H
#define MAIN_H

#include <QObject>
#include <QCoreApplication>
#include <QTime>
#include <QDate>
#include <QtNetwork/QtNetwork>
#include <QtNetwork/QTcpSocket>
#include <QString>
#include <QUrl>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QList>
#include <QFile>
#include <QSslSocket>
#include <QTcpServer>
#include <QByteArray>

#include "payme.h"

#define CANSEL_URL "https://tickets.nikit.uz/api/turnstile/ticket/list"
#define PAYME_URL  "https://tickets.nikit.uz/api/turnstile/ticket/new"
#define UPDATE_URL "https://tickets.nikit.uz/api/turnstile/ticket/update"
#define ECHO "https://postman-echo.com/post"


class myprogram : public QObject
{
    Q_OBJECT
public :
        explicit myprogram(QObject *parent = 0);
public:
    QTcpSocket *TurnSock;

    QTimer *TickTimer;

public:
    void PaymeReceiptsCreate(const QString);
    void PaymeReceiptsPay(const QByteArray);


public slots :
    // Bare Slots
    void TurnSokReadyRead();
    void TurnSokConnected();
    void TurnSokDisconnected();

    // tcp server
    void TcpConnectedToClient();
    void ReadInformation();

    // HTTP
    void PaymeReceiptsCreateRespond(QNetworkReply *reply);
    void PaymeReceiptsPayRespond(QNetworkReply *);
    // Timer
    void TimerTick();

public:
    QTcpServer* tcpserver;
    QTcpSocket* tcpsocket;
//    QNetworkAccessManager * manager;

public:
    Payme* payme;

private:
private:
    const QString id = "1105e3bab097f420a62ced0b";
//    QString token;
    QString idRecipient;

};

#endif // MAIN_H
