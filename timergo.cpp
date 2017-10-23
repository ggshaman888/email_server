#include "timergo.h"
#include "taskthread.h"
#include "sleep.h"

timerGo::timerGo(QObject *parent) : QObject(parent)
{
  Sleep::sleep(interval);
  emit timeIsNow(id);
  //exec();
}

