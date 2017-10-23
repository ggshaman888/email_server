#ifndef QSIMPLESERVER_H
#define QSIMPLESERVER_H

//Подключение используемых в коде классов
#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>
#include <QDateTime>
#include "receiver.h"
#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>
#include <QDateTime>

//Пользовательский класс сервера
class QSimpleServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit QSimpleServer(QObject *parent = 0);

    //Метод, вызываемый при появлении нового соединения,
    //который необходимо переопределить
    void incomingConnection(qint64 handle);
signals:
    void reqestServer(QByteArray, int);


public slots:
    void returnServer(QByteArray, int);
    //Слот обмена информации через сокет
    void onReadyRead();
    //Слот закрывающий сокет
    void onDisconnected();
    void serverGo();

private:
    QByteArray message;

};
/*
//Пользовательский класс сервера
class QSimpleServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit QSimpleServer(QObject *parent = 0);

    //Метод, вызываемый при появлении нового соединения,
    //который необходимо переопределить
    void incomingConnection(int handle);

signals:
    void reqestServer(QByteArray, int);

public slots:
    //Слот обмена данными с сокетом
    void onReadyRead();
    //Слот закрытия сокета
    void onDisconnected();

    void returnServer(QByteArray, int);

private:
    Receiver *r;

};
*/
#endif // QSIMPLESERVER_H
