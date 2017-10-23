#ifndef RECEIVER_H
#define RECEIVER_H

#include <QObject>
#include <QByteArray>

class Receiver : public QObject
{
  Q_OBJECT
public:
  explicit Receiver(QObject *parent = 0);

signals:
  void outServer(QByteArray, int);

public slots:
  void toServer(QByteArray, int);

private:
  int control(QByteArray);

};

#endif // RECEIVER_H
