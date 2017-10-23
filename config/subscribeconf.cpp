#include "config/subscribeconf.h"
#include "config/mailfilterprovider.h"

#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QTextCodec>
#include <QStringList>
#include <QDateTime>

SubscribeConf::SubscribeConf(QObject *parent) : QObject(parent)
{
  iter = -1;
  iter_active = -1;
  date_format = "yyyy-MM-dd";
}

void SubscribeConf::setPath(QString path)
{
  if (path.at(path.size()-1)=='/')
    path.resize(path.size()-1);

  path_subscribes = path;
  QDir dir(path);
  bool ok = dir.exists();
  if (ok)
  {
    dir.setFilter(QDir::AllDirs | QDir::NoSymLinks);
    dir.setSorting(QDir::Name);
    QFileInfoList list = dir.entryInfoList();

    for (int i = 2; i < list.size(); ++i)
    {
      QFileInfo fileInfo = list.at(i);
      all_subscribe << folderToNumber(fileInfo.fileName());
    }
  }
}

int SubscribeConf::size()
{
  size_all = path_subscribes.size();
  return size_all;
}

int SubscribeConf::iterator()
{
  return iter;
}

int SubscribeConf::iteratorActive()
{
  return iter_active;
}

bool SubscribeConf::next()
{
  iter++;
  if(iter>size_all)
    {
      iter=-1;
      return false;
    }
  subscribe = all_subscribe.at(iter);
  QString str, f = path_subscribes; f.append("/");
  f.append(numberToFolder(subscribe)).append("/config.conf");

  return readSubscribe(f);
}

bool SubscribeConf::nextActive()
{
  if(iter_active==-1)
    thatIsActive();

  iter_active++;
  if(iter_active>size_active)
    {
      iter_active=-1;
      return false;
    }

  while(next())
    if(active)
      return true;

  return false;
}

bool SubscribeConf::update()
{
  QStringList list;
  list << "name sender|theme|attach|base|mail log|message|per day|"
          "posted today|date|timer|active\n"
       << updateString();
  QString f = path_subscribes; f.append("/");
  f.append(numberToFolder(subscribe)).append("/config.conf");

  QFile file(f);
  if(file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
      QTextStream out(&file); out.setCodec("UTF-8");
      for(int i=0;i<list.size();i++)
        out << list.at(i);
    }
  file.close();

  return true;
}

bool SubscribeConf::addUp(QString err)
{
  if(err.isEmpty())
    {
      mailReset();
      return update();
    }
  return false;
}

int SubscribeConf::isFolder()
{
  return all_subscribe.at(iter);
}
QString SubscribeConf::isMail()
{
  return isMail("");
}
QString SubscribeConf::isMail(QString provider)
{
  QString file_name = path_subscribes; file_name.append("/");
  file_name.append(numberToFolder(subscribe)).append("/base.csv");

  MailFilterProvider *m = new MailFilterProvider;
  QFile file(file_name);
  bool ok = false;
  if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    while (!ok)
      {
        mail = file.readLine();
        ok = m->mailFilterProvider(mail, provider);
      }
  file.close();

  return mail.remove(10);
}

bool SubscribeConf::mailReset()
{
  QString file_name = path_subscribes; file_name.append("/");
  file_name.append(numberToFolder(subscribe));
  file_name.append("/base.csv");

  QStringList list;
  QString str;
  QFile file;
  file.setFileName(file_name);
  if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    while (!file.atEnd())
      {
        str = file.readLine();
        list << str.remove(10);
      }
  file.close();

  list.removeAll(mail);

  list = removeBadMail(list);

  file.setFileName(file_name);
  if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
      QTextStream out(&file); out.setCodec("UTF-8");
      for(int i=0; i<list.size();i++)
        {
          str = list.at(i);
          str.remove(10);
          QRegExp rx("[^a-zA-Z\\.\\-@0-9_]");
          if(rx.indexIn(str)==-1)
            {
              str.append("\n");
              out << str;
            }
        }
    }

  file.close();

  file_name = path_subscribes; file_name.append("/");
  file_name.append(numberToFolder(subscribe));
  file_name.append("/mail.log");

  file.setFileName(file_name);
  if (file.open(QIODevice::Append | QIODevice::Text))
    {
      QTextStream out(&file); out.setCodec("UTF-8");
      out << mail << "\n";
    }
  file.close();

  return true;
}

int SubscribeConf::sizeActive()
{
  if(iter_active==-1)
    thatIsActive();

  return size_active;
}

int SubscribeConf::folderToNumber(QString name)
{
  QRegExp reg("\\D");
  name.remove(reg);

  if (name[0]=='0') name.remove(0, 1);
  if (name[0]=='0') name.remove(0, 1);
  if (name[0]=='0') name.remove(0, 1);

  return name.toInt();
}

bool SubscribeConf::prepareConfig(QString str)
{
  QStringList list = str.split("|");

  name_sender = list.at(0);
  theme = list.at(1);
  attach = list.at(2);
  base_name = list.at(3);
  mail_log = list.at(4);
  error_log = list.at(5);
  message_path = list.at(6);
  per_day = list.at(7).toInt();
  posted_today = list.at(8).toInt();
  date = QDateTime::fromString(list.at(9), date_format);
  interval = list.at(10).toInt();
  active = list.at(11).toInt();

  isActive();

  if(list.size()>11)
    return true;
  return false;
}

bool SubscribeConf::isActive()
{
  if(!isToday(date))
    {
      date = QDateTime::currentDateTime();
      posted_today = 0;
    }

  if(per_day<posted_today)
    {
      active = true;
      return true;
    }
  else
    {
      active = false;
      return false;
    }
}
bool SubscribeConf::isToday(QDateTime date1)
{
  if (date1.isNull())
    return false;

  QDateTime now = QDateTime::currentDateTime();

  if(date1.toString("dd") == now.toString("dd"))
    return true;
  else
    return false;

}

bool SubscribeConf::thatIsActive()
{
  size_active = 0;
  int x = iter;
  iter = -1;
  for(int i=0;i<all_subscribe.size();i++)
    {
      next();
      if(active)
        size_active++;
    }
  iter = x;

  if(size_active!=0)
    return true;
  return false;
}

bool SubscribeConf::readSubscribe(QString f)
{
  QString str;
  QFile file(f);
  if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    while (!file.atEnd())
      str = file.readLine();
  file.close();

  if(str.isEmpty())
    return false;
  else
    return prepareConfig(str);
}

QString SubscribeConf::updateString()
{
  ++posted_today;
  isActive();
  QString s = "|";
  return QString(name_sender +s+ theme +s+ attach +s+ base_name +s+
                 mail_log +s+ message_path +s+ QString::number(per_day) +s+
                 QString::number(posted_today) +s+
                 QDate::currentDate().toString(date_format) +s+
                 QString::number(interval) +s+ QString::number(active) +"\n");
}

QString SubscribeConf::numberToFolder(int number)
{
  QString str = QString::number(number);
  int x = 4 - str.size();

  for(int i=0; i<x;i++)
    str.prepend("0");

  return str;
}

QStringList SubscribeConf::removeBadMail(QStringList list)
{
  QString file_name = path_subscribes; file_name.append("/");
  file_name.append("/badmail.csv");
  QString str;

  QFile file;
  file.setFileName(file_name);
  if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    while(!file.atEnd())
      {
        str = file.readLine();
        list.removeAll(str.remove(10));
      }
  file.close();

  return list;
}
