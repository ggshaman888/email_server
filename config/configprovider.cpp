#include "configprovider.h"
#include "mutt.h"

#include <QFile>
#include <QTextStream>
#include <QTextCodec>

ConfigProvider::ConfigProvider(QObject *parent) : QObject(parent)
{
  size_all = -1;
  size_active = -1;
  iter = -1;
  iter_active = -1;
  date_format = "yyyy-MM-dd";
  date_time_format = "yyyy-MM-dd hh:mm:ss";

  size();
}

void ConfigProvider::setPath(QString file)
{
  path_file = file;
}

bool ConfigProvider::addUp(QString error_str)
{
  if(!isToday(last_datatime))
    error_today = 0;

  if(error_str.isEmpty())
    {
      posted_today++;
      error_count = 0;
      if(posted_today >= per_day_recomendation)
        active = false;

      last_datatime = QDateTime::currentDateTime();
    }
  else
    {
      ErrorMutt *em = new ErrorMutt;
      if(em->errorStatus(error_str)!="critical")
        {
          max_count;
          min_count_to_error;
          error_today++;
          active = true;
        }
      else
        {
          error_count++;
          error_today++;
          active = false;
        }
    }
  return update();
}

bool ConfigProvider::update()
{
  QStringList list;
  QFile file(path_file);
  if(file.open(QIODevice::ReadOnly | QIODevice::Text))
      while(!file.atEnd())
        list << file.readLine();
  file.close();

  list = updatedList(list);

  if(file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
      QTextStream out(&file); out.setCodec("UTF-8");
      for(int i=0;i<list.size();i++)
        out << list.at(i);
    }
  file.close();
}

bool ConfigProvider::read()
{
  QStringList buf;
  QString str;

  QFile file(path_file);
  if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    str = file.readLine();
    int count=0;
    while (!file.atEnd())
      {
        str = file.readLine();
        if(str.size()>5)
          {
            buf = str.split("|");

            if(buf.at(0).contains("@"))
              if(iter==count)
                {
                  provider = buf.at(0);
                  pass = buf.at(1);
                  max_count = buf.at(2).toInt();
                  min_count_to_error = buf.at(3).toInt();
                  error_count = buf.at(4).toInt();
                  per_day_recomendation = buf.at(5).toInt();
                  posted_today = buf.at(6).toInt();
                  error_today = buf.at(7).toInt();
                  last_datatime = QDateTime::fromString(buf.at(8), date_time_format);
                  active = buf.at(9).toInt();
                }
            if(buf.at(0).contains("@"))
               count++;
          }
      }
    file.close();
}

QString ConfigProvider::updatedString()
{
  if (pass.isEmpty())
    return "";

  QString str = "%1|%2|%3|%4|%5|%6|%7|%8|%9|%10|\n";
  str = str.arg(provider).arg(pass).arg(max_count).arg(min_count_to_error).arg(error_count)
      .arg(per_day_recomendation).arg(posted_today).arg(error_today)
      .arg(last_datatime.toString(date_time_format)).arg(active);

  return str;
}

QStringList ConfigProvider::updatedList(QStringList list_provider_file)
{
  QString buf;
  for(int i=0; i<list_provider_file.size();i++)
    {
      buf = list_provider_file.at(i);
      if(buf.size()<3)
        {
          list_provider_file.removeAt(i);
          i--;
        }
      if (buf.contains(provider))
        list_provider_file.replace(i, updatedString());
    }
  return list_provider_file;
}

bool ConfigProvider::activeTrue(QStringList list)
{
  if(list.at(4).toInt() > 3) // критических ошибок
    return false;

  if(list.at(6).toInt() < list.at(5).toInt()) // отправленно от лимита
    return true;
  else
    return false;
}

bool ConfigProvider::isToday(QDateTime date1)
{
  if (date1.isNull())
    return false;

  QDateTime now = QDateTime::currentDateTime();

  if(date1.toString("dd") == now.toString("dd"))
    return true;
  else
    return false;

}
bool ConfigProvider::readActive()
{
  QStringList buf;
  QString str;

  QFile file(path_file);
  if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    str = file.readLine();
    int count=0;
    while (!file.atEnd())
      {
        str = file.readLine();
        if(str.size()>5)
          {
            buf = str.split("|");

            if(buf.at(0).contains("@"))
              if(activeTrue(buf))
                {
                  if(iter_active==count)
                    {
                      provider = buf.at(0);
                      pass = buf.at(1);
                      max_count = buf.at(2).toInt();
                      min_count_to_error = buf.at(3).toInt();
                      error_count = buf.at(4).toInt();
                      per_day_recomendation = buf.at(5).toInt();
                      posted_today = buf.at(6).toInt();
                      error_today = buf.at(7).toInt();
                      last_datatime = QDateTime::fromString(buf.at(8), date_time_format);
                      active = true;
                    }
                  if(buf.at(0).contains("@"))
                    count++;
                }
          }
      }
    file.close();
}

int ConfigProvider::size()
{
  QString str;
  if(size_all==-1)
    {
      QStringList buf;

      QFile file(path_file);
      if (file.open(QIODevice::ReadOnly | QIODevice::Text))
        str = file.readLine();
        while (!file.atEnd())
          {
            str = file.readLine();
            if(str.size()>5)
              {
                buf = str.split("|");

                if(buf.at(0).contains("@"))
                  {
                    size_all++;
                    if(activeTrue(buf))
                      size_active++;
                  }
               }
          }
        file.close();
    }
  return size_all;
}

int ConfigProvider::sizeActive()
{
  if(size_active==-1)
    size();
  return size_active;
}

int ConfigProvider::iterator()
{
  return iter;
}

int ConfigProvider::iteratorActive()
{
  return iter_active;
}

bool ConfigProvider::next()
{
  iter++;
  if(iter>size_all)
    {
      iter=-1;
      return false;
    }
  read();
  return true;
}

bool ConfigProvider::addNew()
{
  QFile file(path_file);
  if(file.open(QIODevice::Append | QIODevice::Text))
    {
      QTextStream out(&file); out.setCodec("UTF-8");
      out << updatedString();
    }
  file.close();
  return true;
}

bool ConfigProvider::removeIs()
{
  pass = "";
  return update();
}

bool ConfigProvider::setIterProvider(QString prov)
{
  iter = -1;
  while(next())
    if(provider.contains(prov))
      return true;
  return false;
}

bool ConfigProvider::nextActive()
{
  iter_active++;
  if(iter_active>size_active)
    {
      iter_active=-1;
      return false;
    }
  readActive();
  return true;
}
