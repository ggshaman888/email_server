#include <QFile>
#include <QTextStream>
#include "log.h"

Log::Log(QObject *parent) : QObject(parent)
{

}

void Log::setLogFile(QString file)
{
  log_file = file;
}

void Log::addLog(QString str)
{
  QFile file;
  file.setFileName(log_file);
  file.open(QIODevice::Append | QIODevice::Text);
  QTextStream out(&file);
  out << str;
  file.close();
}

