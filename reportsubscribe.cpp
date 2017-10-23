#include "reportsubscribe.h"
#include <subscribeconfig.h>
#include <QStringList>
#include <QFile>
#include <QTextCodec>
#include <QTextStream>
#include <QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlDriver>
#include <QtSql/QtSql>
#include <QtConcurrent/QtConcurrent>

ReportSubscribe::ReportSubscribe(QObject *parent) : QObject(parent)
{
  iter_log = 0;
  path_s = "/home/alexandr/mutt/subscribe";
//  path_sqlite = "/home/alexandr/mutt/subscribe/log.db";
  path_sqlite = "/mnt/tmpfs/log.db";
}

void ReportSubscribe::reportAllSubscribe()
{
  readLogSubscribe();
  toLogSQLite();
//  writeAllReportSubscribe();
  qDebug() << "отчёт готов";
}

void ReportSubscribe::writeAllReportSubscribe()
{
  sortFile();

  QFile f(QString(path_s + "/report_all.csv"));
  if(f.open(QIODevice::Append | QIODevice::Text))
    {
      QTextStream out(&f); out.setCodec("UTF-8");
      for(int i=0;i<list_file.size();i++)
          out << list_file.at(i);
    }
  f.close();
}

void ReportSubscribe::toLogSQLite()
{
  createConnectionMYSQLite (path_sqlite);
  QSqlQuery query_sqlite(QSqlDatabase::database("dbsqlite"));
  SubscribeConfig *s = new SubscribeConfig;
  QString data , crit;

  qDebug()<< log.size() << QDateTime::currentDateTime();

  for(int i=0;i<log.size();i++)
    {
      if(i==1000)
        qDebug()<< "10000" << QDateTime::currentDateTime();

      data = log.at(i).datetime; data.resize(10);
      if(s->errorCriticalMessage(log.at(i).error)) crit="1";
      else crit="0";

      query_sqlite.prepare(""
       "INSERT INTO log (provider, email, data1, error, critical, subscribe) "
                    "VALUES (?, ?, ?, ?, ?, ?)");
      query_sqlite.addBindValue(log.at(i).provider);
      query_sqlite.addBindValue(log.at(i).email);
      query_sqlite.addBindValue(data);
      query_sqlite.addBindValue(log.at(i).error);
      query_sqlite.addBindValue(crit);
      query_sqlite.addBindValue(log.at(i).subscribe);
      query_sqlite.exec();
      query_sqlite.lastError();

    }
  qDebug()<< "end" << QDateTime::currentDateTime();
}

void ReportSubscribe::onceNewStrToFile(QString provider, QString data, QString error)
{
  if(error.isEmpty())
    list_file.append(QString(provider + "|" + data + "|1|0|0|1|1|1|\n"));
  else
    {
      SubscribeConfig *s = new SubscribeConfig;
      if(s->errorCriticalMessage(error))
        list_file.append(QString(provider + "|" + data + "|0|1|1|0|0|0|\n"));
      else
        list_file.append(QString(provider + "|" + data + "|0|1|0|0|0|0|\n"));
    }
}

void ReportSubscribe::strParsing(QString str)
{
  QStringList list = str.split("|");
  list.replaceInStrings("\n", "");

  LogLog l;
  l.provider = list.at(1);
  l.email = list.at(3);
  l.datetime = list.at(5);
  l.error = list.at(7);

  l.error.replace(10, " ");
  l.provider.replace(" ", "");
  l.email.replace(" ", "");
  l.datetime.replace(" ", "");

  l.subscribe = thisIsSubscribe(l.provider);

  log << l;
}

void ReportSubscribe::readSubscribeLog(QString path_s)
{
  QFile f(path_s);
  if(f.open(QIODevice::ReadOnly | QIODevice::Text))
    while(!f.atEnd())
      strParsing(f.readLine());
  f.close();
}

void ReportSubscribe::sortFile()
{
  for(int i=0;i<log.size();i++)
    addToListFile(i);
}

void ReportSubscribe::addToListFile(int i)
{
  QString provider = log.at(i).provider;
  QString data = log.at(i).datetime;
//  QString email = log.at(i).email;
  QString error = log.at(i).error;
  data.resize(10);
  QString str;

  if(list_file.isEmpty())
    onceNewStrToFile(provider, data, error);
  else
    {
      for(int j=0; j<list_file.size(); j++)
        {
          str = list_file.at(j);
          if(str.contains(provider))
            if(str.contains(data))
              list_file.replace(j, strToLog(str, i));
          else
            onceNewStrToFile(provider, data, error);
        }
    }
}

QString ReportSubscribe::strToLog(QString str, int i)
{
  QStringList l = str.split("|");
  int count = l.at(2).toInt();
  int err_day = l.at(3).toInt();
  int err_crit = l.at(4).toInt();
  int min = l.at(5).toInt();
  int max = l.at(6).toInt();
  int count_status = l.at(7).toInt();

  if(log.at(i).error.isEmpty())
    {
      count++;
      count_status++;
      if (count_status>max)
        max = count_status;
    }
  else
    {
      SubscribeConfig *s = new SubscribeConfig;
      if(s->errorCriticalMessage(log.at(i).error))
        {
          if(min==0 or min>count_status)
            min = count_status;

          err_crit++;
          err_day++;
        }
      else
        err_day++;
    }
  return QString( l.at(0) + "|" + l.at(1) + "|"
                + QString::number(count) +"|"
                + QString::number(err_day) + "|"
                + QString::number(err_crit) + "|"
                + QString::number(min) + "|"
                + QString::number(max)+ "|"
                  + QString::number(count_status) + "|\n");
}

QString ReportSubscribe::thisIsSubscribe(QString provider)
{
  SubscribeConfig *cnf = new SubscribeConfig;

  QString buf, str;
  cnf->setPathSubscribe(path_s);
  QList<int> slist = cnf->allSubscribe();

  for(int i=0; i<slist.size();i++)
    {
      str = QString(path_s + "/" + cnf->numberToFolder(slist.at(i)) + "/providers.conf");
      if (defineSubscribe(str, provider))
        return QString::number(slist.at(i));
    }
  return "";
}

bool ReportSubscribe::defineSubscribe(QString file, QString provider)
{
 // if(provider.conains())
  QString str;
  QFile f(file);
  if(f.open(QIODevice::ReadOnly | QIODevice::Text))
    {
      while(!f.atEnd())
        {
          str = f.readLine();
          if(str.contains(provider))
            {
              f.close();
              return true;
            }
        }
    }
  f.close();
  return false;
}

void ReportSubscribe::createTmpfs()
{

}

void ReportSubscribe::readLogSubscribe()
{
  SubscribeConfig *cnf = new SubscribeConfig;

  QString str;
  cnf->setPathSubscribe(path_s);
  QList<int> slist = cnf->allSubscribe();

  for(int i=0; i<slist.size();i++)
    readSubscribeLog(QString(path_s + "/" + cnf->numberToFolder(slist.at(i))
                             + "/log.log"));
}

