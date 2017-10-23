#include <QStringList>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <subscribeconfig.h>
#include "taskconfig.h"
#include "QDebug"
#include <sleep.h>
#include <config/mutt.h>

SubscribeConfig::SubscribeConfig(QObject *parent) : QObject(parent)
{
  all_active_provider = 0;
  carriage_active_subscribe = -1;
  carriage_active_provider = -1;
  date_format = "yyyy-MM-dd";
  date_time_format = "yyyy-MM-dd hh:mm:ss";
}


void SubscribeConfig::setPathSubscribe(QString path)
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

void SubscribeConfig::setActiveSubscribe()
{
  for(int i=0; i<all_subscribe.size();i++)
    if (activeSubscribe(all_subscribe.at(i)))
      active_subscribe << all_subscribe.at(i);

  setAllActiveProvider();
}

void SubscribeConfig::setProvider(int subscribe_number = 0)
{
  if (active_subscribe.size()==carriage_active_subscribe)
    {
      setPathSubscribe(path_subscribes);
      carriage_active_subscribe = 0;
    }

  if(carriage_active_subscribe==-1)
    nextActiveSubscribe();
  provider_all.clear();
  ConfigProvider provider_elements;

  QString file_name;
  if (subscribe_number == 0)
    {
      file_name = path_subscribes; file_name.append("/");
      file_name.append(numberToFolder(active_subscribe.at(carriage_active_subscribe)));
      file_name.append("/providers.conf");
    }
  else
    {
      file_name = path_subscribes; file_name.append("/");
      file_name.append(numberToFolder(subscribe_number));
      file_name.append("/providers.conf");
    }

  QStringList buf;
  QString str;

  QFile file;
  file.setFileName(file_name);
  if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    str = file.readLine();
    while (!file.atEnd())
      {
        str = file.readLine();
        if(str.size()>5)
          {
            buf = str.split("|");

            provider_elements.provider = buf.at(0);
            provider_elements.pass = buf.at(1);
            provider_elements.max_count = buf.at(2).toInt();
            provider_elements.min_count_to_error = buf.at(3).toInt();
            provider_elements.error_count = buf.at(4).toInt();
            provider_elements.per_day_recomendation = buf.at(5).toInt();
            provider_elements.posted_today = buf.at(6).toInt();
            provider_elements.error_today = buf.at(7).toInt();
            provider_elements.last_datatime = QDateTime::fromString(buf.at(8), date_time_format);
            provider_elements.active = buf.at(9).toInt();

            provider_all << setProviderElements(provider_elements);
          }
      }
  file.close();
}

bool SubscribeConfig::nextActiveProvider()
{  
  if (carriage_active_provider<provider_all.size())
    {
      int i = ++carriage_active_provider;
      for(; i<provider_all.size(); i++)
        if (provider_all.at(i).active)
          {
            carriage_active_provider = i;
            return 1;
          }
    }
  if (carriage_active_provider==provider_all.size()-1)
    {
      carriage_active_provider = 0;
      return 1;
    }

  carriage_active_provider = -1;
  return 0;
}

bool SubscribeConfig::nextActiveSubscribe()
{  
  ++carriage_active_subscribe;

  if (carriage_active_subscribe<active_subscribe.size())
    return 1;

  carriage_active_provider = -1;
  return 0;
}

bool SubscribeConfig::nextSet()
{
  if(active_subscribe.isEmpty())
    setActiveSubscribe();

 // if(all_active_provider==0)
 //   {
 //     qDebug() << "активные провайдеры отсутствуют";
 //     Sleep::msleep(200000);
 //   }
  if (provider_all.isEmpty())
    setProvider(0);

  if (!nextActiveProvider())
    {
      nextActiveSubscribe();
      setProvider(0);
      return nextSet();
    }
  else
    return true;
  return false;
}

bool SubscribeConfig::next()
{
  if(activeProvider())
    return nextActiveProviderNew();
  else
    return false;
}

QList<int> SubscribeConfig::allSubscribe()
{
  return all_subscribe;
}

QList<SubscribeConfig::ConfigProvider> SubscribeConfig::providersSubscribe(int number)
{
  setProvider(number);
  return provider_all;
}

QString SubscribeConfig::isMail()
{
  QString file_name = path_subscribes; file_name.append("/");
  file_name.append(numberToFolder(active_subscribe.at(carriage_active_subscribe)));
  file_name.append("/base.csv");

  QFile file;
  file.setFileName(file_name);
  bool ok = false; int c = 0;
  if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    while (!ok)
      {
        c++;
        mail = file.readLine();
        ok = mailFilterProvider(mail);
      }
  file.close();

  return mail.remove(10);
}

SubscribeConfig::ConfigProvider SubscribeConfig::isProvider()
{
  return provider_all.at(carriage_active_provider);
}

SubscribeConfig::ConfigIs SubscribeConfig::isSubscribe()
{
  if(carriage_active_subscribe==-1)
    nextActiveSubscribe();

  QString file_name = path_subscribes; file_name.append("/");
  file_name.append(numberToFolder(active_subscribe.at(carriage_active_subscribe)));
  file_name.append("/config.conf");

  QString str;

  QFile file;
  file.setFileName(file_name);
  if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    while (!file.atEnd())
      str = file.readLine();
  file.close();

  QStringList list = str.split("|");

  this_subscribe.name_sender = list.at(0);
  this_subscribe.theme = list.at(1);
  this_subscribe.attach = list.at(2);
  this_subscribe.base_name = list.at(3);
  this_subscribe.mail_log = list.at(4);
  this_subscribe.error_log = list.at(5);
  this_subscribe.message_path = numberToFolder(active_subscribe.at(carriage_active_subscribe));
  this_subscribe.message_path.append("/").append(list.at(6));
  this_subscribe.per_day = list.at(7).toInt();
  this_subscribe.interval = list.at(8).toInt();
  this_subscribe.active = list.at(9).toInt();

  return this_subscribe;
}

void SubscribeConfig::updateLog(QString error_out)
{
  if (error_out.isEmpty())
      mailReset();
  toLog(error_out);
  providerUpdateStatistic(error_out);

}

bool SubscribeConfig::activeSubscribe(int name)
{
  QString file_name = path_subscribes; file_name.append("/");
  file_name.append(numberToFolder(name)); file_name.append("/config.conf");

  QString str;

  QFile file(file_name);
  //file.setFileName(file_name);
  if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    while (!file.atEnd())
      str = file.readLine();
  file.close();

  if (str.isEmpty())
    qDebug() << "Конфигурация отсутствует!" << file_name;


  QStringList list = str.split("|");

  int x = list.at(list.size()-2).toInt();
  return x;
}

void SubscribeConfig::mailReset()
{
  QString file_name = path_subscribes; file_name.append("/");
  file_name.append(numberToFolder(active_subscribe.at(carriage_active_subscribe)));
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
  list.removeDuplicates();

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
  file_name.append(numberToFolder(active_subscribe.at(carriage_active_subscribe)));
  file_name.append("/mail.log");

  file.setFileName(file_name);
  if (file.open(QIODevice::Append | QIODevice::Text))
    {
      QTextStream out(&file); out.setCodec("UTF-8");
      out << mail << "\n";
    }
  file.close();

}

void SubscribeConfig::toLog(QString error_out)
{
  QString str = "provider | %1 | email | %2 | datetime | %3 | error | %4 |\n";
  str = str.arg(provider_all.at(carriage_active_provider).provider)
      .arg(mail).arg(QDateTime::currentDateTime().toString(date_time_format))
      .arg(error_out.replace(10, " "));

  QString file_name = path_subscribes; file_name.append("/");
  file_name.append(numberToFolder(active_subscribe.at(carriage_active_subscribe)));
  file_name.append("/log.log");

  QFile file;
  file.setFileName(file_name);
  if(file.open(QIODevice::Append | QIODevice::Text))
    {
      QTextStream out(&file); out.setCodec("UTF-8");
      out << str;
    }
  file.close();
}

void SubscribeConfig::providerUpdateStatistic(QString error_out)
{
  QList<ConfigProvider>::iterator cnf = provider_all.begin();
  for(int i=0; i<carriage_active_provider;i++)
    ++cnf;

  ConfigProvider &cnf_provider = *cnf;

  if(!isToday(cnf_provider.last_datatime))
    {
      cnf_provider.last_datatime = QDateTime::currentDateTime();
      if (error_out.isEmpty())
        {
          cnf_provider.error_count=0;
          cnf_provider.posted_today=0;
        }
      else
        {
          if (errorCriticalMessage(error_out))
            {
              cnf_provider.error_count++;
              cnf_provider.error_today=1;
              cnf_provider.active = 0;
            }
          cnf_provider.error_today=1;
        }
    }
    else
    {
      if (error_out.isEmpty())
        {
          cnf_provider.error_count=0;
          cnf_provider.posted_today++;
        }
      else
        {
          if (errorCriticalMessage(error_out))
            {
              cnf_provider.error_count++;
              cnf_provider.error_today++;
              cnf_provider.active = 0;
            }
          cnf_provider.error_today++;
        }
    }

  providerReplase(cnf_provider);
}

bool SubscribeConfig::isToday(QDateTime date1)
{
  if (date1.isNull())
    return true;

  QDateTime now = QDateTime::currentDateTime();

  if(date1.toString("dd") == now.toString("dd"))
    return true;

  else
    return false;
}

bool SubscribeConfig::errorCriticalMessage(QString error_out)
{
  if (error_out.contains("SMTP session failed: 553 5.1.2 RFC-5321 address"))
    {
      mailBad();
      return false;
    }
  if (error_out.contains("SMTP session failed: 550"))
    {
      mailBad();
      return false;
    }
  if(error_out.contains("SASL authentication failed"))
    return true;
  if(error_out.contains("SPAM"))
    return true;
  if(error_out.contains("Daily user sending quota exceeded"))
    return true;
  if(error_out.contains("SMTP session failed"))
    return true;

  if (error_out.contains("No recipients were specified"))
    return false;
  if (error_out.contains("Could not find the host"))
    return false;

  return false;
}

void SubscribeConfig::providerReplase(SubscribeConfig::ConfigProvider cnf_provider)
{
  QString file_name = path_subscribes; file_name.append("/");
  file_name.append(numberToFolder(active_subscribe.at(carriage_active_subscribe)));
  file_name.append("/providers.conf");

  QStringList list;
  QFile file;
  file.setFileName(file_name);
  if(file.open(QIODevice::ReadOnly | QIODevice::Text))
      while(!file.atEnd())
        list << file.readLine();
  file.close();

  QString str = "%1|%2|%3|%4|%5|%6|%7|%8|%9|%10|\n";
  str = str.arg(cnf_provider.provider).arg(cnf_provider.pass).arg(cnf_provider.max_count)
      .arg(cnf_provider.min_count_to_error).arg(cnf_provider.error_count)
      .arg(cnf_provider.per_day_recomendation).arg(cnf_provider.posted_today)
      .arg(cnf_provider.error_today).arg(cnf_provider.last_datatime.toString(date_time_format))
      .arg(cnf_provider.active);


  QString buf;
  for(int i=0; i<list.size();i++)
    {
      buf = list.at(i);
      if (buf.contains(cnf_provider.provider))
        list.replace(i, str);
    }


  file.setFileName(file_name);
  if(file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
      QTextStream out(&file); out.setCodec("UTF-8");
      for(int i=0;i<list.size();i++)
        out << list.at(i);
    }
  file.close();
}

QString SubscribeConfig::numberToFolder(int number)
{
  QString str = QString::number(number);
  int x = 4 - str.size();

  for(int i=0; i<x;i++)
    str.prepend("0");

  return str;
}

int SubscribeConfig::folderToNumber(QString name)
{
  QRegExp reg("\\D");
  name.remove(reg);

  if (name[0]=='0') name.remove(0, 1);
  if (name[0]=='0') name.remove(0, 1);
  if (name[0]=='0') name.remove(0, 1);

  return name.toInt();
}

bool SubscribeConfig::mailFilterProvider(QString mail)
{
  mail.remove(10);
  if (mail.size()<4)
    return 0;

  QString provider = provider_all.at(carriage_active_provider).provider;

  if(mailRuProvider(mail))
    {
      QStringList providers;
      providers << "poll.dekor@gmail.com"
                << "tosdoma.3.16.pobeda@gmail.com"
                << "vin.test.tlt@gmail.com"
                << "vin.standard.tlt@gmail.com";
      int size = providers.size();
      for(int i=0; i<size; ++i)
        if (provider.contains(providers.at(i)))
          return 1;
    }

  QRegExp rx("[^a-zA-Z\\-\\.@0-9_]");

  int x = rx.indexIn(mail);
  if(rx.indexIn(mail)!=-1)
    return 0;

  return 1;


//  if (mail.contains("-"))
//    {
//      if(provider.contains("@bk.ru"))
//        return 1;
//      else
//        return 0;
//    }
//  else
//    {

//    }
}

void SubscribeConfig::mailBad()
{
  QString file_name = path_subscribes; file_name.append("/");
  file_name.append("/badmail.csv");

  QFile file;
  file.setFileName(file_name);
  if (file.open(QIODevice::Append | QIODevice::Text))
    {
      QTextStream out(&file); out.setCodec("UTF-8");
      out << mail << "\n";
    }
  file.close();
  mailReset();
}

QStringList SubscribeConfig::removeBadMail(QStringList list)
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

void SubscribeConfig::toProviderNoActive(QString provider, QString pass)
{
  QString file_name = path_subscribes; file_name.append("/");
  file_name.append("/provider_no_active.csv");
  QString str;
  QStringList list;
  QFile file;
  file.setFileName(file_name);
  if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    while(!file.atEnd())
      {
        str = file.readLine();
        list << str.remove(10);
      }
  file.close();
  list.removeDuplicates();

  if(!list.contains(provider))
    list << provider.append(" | ").append(pass).append("\n");

  if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
      QTextStream out(&file); out.setCodec("UTF-8");
      for(int i=0;i<list.size();i++)
        out << list.at(i) << "\n";
    }

  file.close();
}

void SubscribeConfig::setAllActiveProvider()
{
  all_active_provider = 0;
  for(int i=0; i<all_subscribe.size(); i++)
    {
      setProvider(0);
      for(int j=0; j<provider_all.size();j++)
          if (provider_all.at(j).active)
            all_active_provider++;
    }
}

SubscribeConfig::ConfigProvider SubscribeConfig::setProviderElements(SubscribeConfig::ConfigProvider elements)
{
  if (elements.last_datatime.isNull())
    elements.last_datatime = QDateTime::currentDateTime();

  if(!isToday(elements.last_datatime))
    {
      elements.error_today = 0;
      elements.posted_today = 0;

      if(elements.error_count > 3)
        elements.active = 0;

      elements.active = 1;
    }
  else
    {
      if(elements.posted_today<elements.per_day_recomendation)
        elements.active = 1;
      else
        elements.active = 0;

      if(elements.error_count > 3)
        elements.active = 0;
    }

  if(elements.active == false)
    toProviderNoActive(elements.provider, elements.pass);

  return elements;
}

bool SubscribeConfig::activeProvider()
{
  setActiveSubscribeNew();
  for(int i=0; i<active_subscribe.size();i++)
    {
      if(setProviderNew(active_subscribe.at(i)))
        return true;
      else
        {
          active_subscribe.removeAt(i);
          --i;
        }
    }
  return false;
}

void SubscribeConfig::setActiveSubscribeNew()
{
  for(int i=0; i<all_subscribe.size();i++)
    if (activeSubscribe(all_subscribe.at(i)))
      active_subscribe << all_subscribe.at(i);
}

bool SubscribeConfig::setProviderNew(int number)
{
  setProvider(number);

  if(provider_all.isEmpty())
    return false;

  return true;
}

bool SubscribeConfig::nextActiveProviderNew()
{
  if(!last_provider.isEmpty())
    {
      carriage_active_provider = 0;
      last_provider = provider_all.at(0).provider;
      return true;
    }
  else
    {
      carriage_active_provider++;
      last_provider = provider_all.at(carriage_active_provider).provider;
      if(!last_provider.isEmpty())
        return true;
      else
        return false;
    }
  return false;
}

bool SubscribeConfig::mailRuProvider(QString mail)
{
  if (mail.contains("@mail.ru")) return true;
  if (mail.contains("@bk.ru")) return true;
  if (mail.contains("@list.ru")) return true;
  if (mail.contains("@inbox.ru")) return true;
  return false;
}
