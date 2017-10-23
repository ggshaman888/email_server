#include <QStringList>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QDebug>

#include "taskconfig.h"

TaskConfig::TaskConfig(QObject *parent) : QObject(parent)
{
date_format = "yyyy-MM-dd hh:mm:ss";
}

void TaskConfig::setConfig()
{
  QFile file(is_file_config); // создаем объект класса QFile

  if (file.open(QIODevice::WriteOnly))
    {
      QTextStream out(&file); out.setCodec("UTF-8");
      QStringList list = config();

      int size = list.size();
      for (int i=0; i<size; i++)
        out << list.at(i);
    }
  file.close();
}

void TaskConfig::readConfig(QString file_path)
{
  is_folder = file_path;
  is_file_config = file_path; is_file_config.append("config.conf");

  QFile file(is_file_config); // создаем объект класса QFile
  QStringList list, list_result;

  if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    while (!file.atEnd())
      list << file.readLine();
  file.close();

  QString str = list.at(1);
  list_result = str.split("|");

  is_name_sender = list_result.at(0);
  is_theme = list_result.at(1);
  is_login = list_result.at(2);
  is_password = list_result.at(3);
  if (list_result.at(4) != "none")
    {
      is_file_attach = file_path; is_file_attach.append(list_result.at(4));
    }
  is_file_base_mail = file_path; is_file_base_mail.append(list_result.at(5));
  is_file_mail_log = file_path; is_file_mail_log.append(list_result.at(6));
  is_file_error_log  = file_path; is_file_error_log.append(list_result.at(7));
  is_file_message = file_path; is_file_message.append(list_result.at(8));
  is_per_day = list_result.at(9).toInt();
  is_timer = list_result.at(10).toInt();
  is_active = list_result.at(11).toInt();
}

void TaskConfig::readConfigTasks(QString path_t)
{
  path_tasks = path_t;
}

QStringList TaskConfig::isErrorLog()
{
  QFile file(is_file_error_log); // создаем объект класса QFile
  QStringList list, list_result;

  if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    while (!file.atEnd())
      list << file.readLine();
  file.close();

  QString str, buf; bool x = false;
  int size = list.size();
  for(int i=0; i<size; i++)
    {
      if (!x)
        {
          if (!str.isEmpty())
            {
              list_result << str;
              x = false;
              str.clear();
            }

          buf = list.at(i);
          if(buf.indexOf("письма от провайдера")>0)
            {
              str.append(list.at(i));
              x = true;
            }
        }
      else
        {
          buf = list.at(i);
          if(buf.indexOf("письма от провайдера")>0)
            {
              list_result << str;
              str.clear();

              str.append(list.at(i));
              x = true;
            }
          else
            str.append(list.at(i));
        }
    }

  return list_result;
}

QStringList TaskConfig::isMailLog()
{
  QFile file(is_file_mail_log); // создаем объект класса QFile
  QStringList list, list_result;

  if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    while (!file.atEnd())
      list << file.readLine();
  file.close();

  int size = list.size();
  for(int i=0; i<size; i++)
    list_result << clrMail(list, i);

  return list_result;
}

QStringList TaskConfig::isMailBase()
{
  QFile file(is_file_base_mail); // создаем объект класса QFile
  QStringList list, list_result;

  if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    while (!file.atEnd())
      list << file.readLine();
  file.close();

  QString str;
  int size = list.size();
  for(int i=0; i<size; i++)
    {
      str = clrMail(list, i);
      list_result << str;
    }
  return list_result;
}

QStringList TaskConfig::isMailErrorLogTrue()
{
  QStringList log = isErrorLog();
  QStringList mail;

  for(int i=0;i<log.size();i++)
    mail << strErrorLogToMail(log.at(i));

  return mail;
}

int TaskConfig::isTimeStart(QStringList name_folder)
{
  int result, count = 0;
  for(int i=0;i<name_folder.size();i++)
    {
      result = isTimeStart(clrFolderName(name_folder.at(i)));

      if (result > count)
        count = result;
    }

  return count;
}

int TaskConfig::isTimeStart(QString name_folder)
{
  this->readConfig(path_tasks.append(name_folder).append("/"));
  this->setConfig();
  QStringList list = isErrorLog();
  QString str = list.last();
  list.clear();
  list = str.split(" | ");
  str = list.at(7);

  QDateTime date = QDateTime::fromString (str, date_format);
  int x = QDateTime::currentMSecsSinceEpoch() - date.currentMSecsSinceEpoch();

  return x;
}

void TaskConfig::addMailBase(QStringList list)
{
  QFile file(is_file_base_mail); // создаем объект класса QFile

  if (file.open(QIODevice::Append))
    {
      QTextStream out(&file); out.setCodec("UTF-8");

      int size = list.size();
      for (int i=0; i<size; i++)
        out << list.at(i);
    }
  file.close();
}

void TaskConfig::clearMailLog()
{
  QFile file(is_file_mail_log);
  if(file.open(QIODevice::WriteOnly | QIODevice::Truncate))
    file.close();
}

void TaskConfig::clearErrorLog()
{
  QFile file(is_file_error_log);
  if (file.open(QIODevice::WriteOnly | QIODevice::Truncate))
    file.close();
}

void TaskConfig::clearMailBase()
{
  QFile file(is_file_base_mail);
  if (file.open(QIODevice::WriteOnly | QIODevice::Truncate))
    file.close();
}

void TaskConfig::clearAll()
{
  clearMailLog();
  clearErrorLog();
  clearMailBase();
}

QString TaskConfig::readStrError(QStringList list, int x)
{
  QString buf, str;

  buf = list.at(x); str.clear();
  if (buf.indexOf("письма от провайдера")>-1)
    {
      str.append(buf); buf = " "; x++;

      if (x<list.size())
        {
          while (buf.indexOf("письма от провайдера")==-1)
            {
              buf = list.at(x);
              str.append(buf); str.append(" ");
              ++x;

              if (x==list.size())
                break;
            }
        }
    }
  if(str.isEmpty())
    {
        str = readStrError(list, ++x);
    }

  return str;
}

QString TaskConfig::clrMail(QStringList list, int x)
{
  QString buf = list.at(x);
  buf.trimmed();
  return buf;
}

QStringList TaskConfig::config()
{
  QString str;
  QStringList config;

  str = "name sender|theme|mail-sender|pass-mail|attach|base|mail log|error log|message|per day|timer|active|";
  config << str;

  str = is_name_sender; str.append("|");
  str = is_theme; str.append("|");
  str = is_login; str.append("|");
  str = is_password; str.append("|");
  str = is_file_attach; str.append("|");
  str = is_file_base_mail; str.append("|");
  str = is_file_mail_log; str.append("|");
  str = is_file_error_log; str.append("|");
  str = is_file_message; str.append("|");
  str = QString::number(is_per_day); str.append("|");
  str = QString::number(is_timer); str.append("|");
  str = QString::number(is_active); str.append("|");

  config << str;

  return config;
}

QString TaskConfig::strErrorLogToMail(QString str)
{
  QStringList list = str.split("|");
  str = list.at(5);
  str = str.replace(" ", "");
  return str;
}

QString TaskConfig::clrFolderName(QString str)
{
  QStringList list = str.split("/");
  if (list.size()>1)
    if (list.last()=="/")
      list.removeLast();

  return list.last();
}



