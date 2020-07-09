#ifndef PAYME_H
#define PAYME_H

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

class Payme: public QObject
{
    Q_OBJECT
public :
    explicit Payme(QObject *parent = 0);


    void create();
    void pay();


private:
    QString id;
    QString token;
    QString idRecipient;

public slots :
    void PaymeCreateRespond(QNetworkReply *reply);



};

#endif // PAYME_H
