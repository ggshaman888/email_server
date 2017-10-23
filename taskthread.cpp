#include "taskthread.h"
#include <QObject>

/*
TaskThred::TaskTred(QObject *parent = 0):
  QObject(parent)
{

}
*/
void TaskThred::run()
{

  exec();
}

