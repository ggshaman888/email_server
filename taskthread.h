
#ifndef TASKTRED_H
#define TASKTRED_H

#include <QThread>
#include <QObject>

class TaskThred : public QThread
{
  Q_OBJECT

public:
  TaskThred(QObject *parent = 0):
      QThread(parent)
  {}

  void run();
  int interval;
  int id;

signals:
  void timeIsNow(int);


};



#endif // TASKTRED_H
