#include <QDebug>
#include "qsimpleserver.h"
#include "receiver.h"
#include "taskthread.h"

QSimpleServer::QSimpleServer(QObject *parent) :
    QTcpServer(parent)
{
    //Запуск сервера на прослушивание на порту 80
    if(listen(QHostAddress::Any, 1080)){
        qDebug() << "Listening...";
    }
    else {
        qDebug() << "Error while starting: " << errorString();
    }

    Receiver *r = new Receiver;
    connect(this, SIGNAL(reqestServer(QByteArray,int)), r, SLOT(toServer(QByteArray,int)));
    connect(r, SIGNAL(outServer(QByteArray,int)), this, SLOT(returnServer(QByteArray,int)));
}

void QSimpleServer::incomingConnection(qint64 handle)
{
    //Создание объекта сокета
    QTcpSocket* socket = new QTcpSocket();
    //Сопоставление объекта сокета с системным сокетом по дескриптору
    socket->setSocketDescriptor(handle);

    //Соединение сигналов сокета с одноименными слотами
    connect(socket, SIGNAL(readyRead()), this, SLOT(serverGo()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
}

void QSimpleServer::onReadyRead()
{
    //Получение указателя на слот, испустивший сигнал
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
    //Чтение и вывод информации из сокета
    QString response = "HTTP/1.1 200 OK\r\n\r\n%1";
    QString str = message;
    socket->write(response.arg(str).toUtf8());

    socket->disconnectFromHost();
    //Отсоединение сокета
}

void QSimpleServer::onDisconnected()
{
    //Получение указателя на слот, испстивший сигнал
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
    //Закрытие сокета
    socket->close();
    //Метод отсроченного удаления объекта
    socket->deleteLater();
}

void QSimpleServer::serverGo()
{
  QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
  emit reqestServer(socket->readAll(), 1);
 // socket->disconnectFromHost();
 // onReadyRead();
}

void QSimpleServer::returnServer(QByteArray mess, int x)
{
  qDebug() << x;
  message = mess;
  onReadyRead();
}
