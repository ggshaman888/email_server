#ifndef TIMERGO_H
#define TIMERGO_H

#include <QObject>

class timerGo : public QObject
{
  Q_OBJECT
public:
  explicit timerGo(QObject *parent = 0);


  int interval;
  int id;

signals:
  void timeIsNow(int);
public slots:
};

#endif // TIMERGO_H
