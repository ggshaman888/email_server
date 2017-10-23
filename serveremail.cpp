#include <QStringList>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QQueue>
#include <QMap>
#include <QDebug>
#include <QMapIterator>
#include <QProcess>
#include "serveremail.h"
#include "taskthread.h"
#include <iostream>
#include "dbreload.h"
#include "subscribeconfig.h"
#include "log.h"
#include "sleep.h"

using namespace std;

ServerEmail::ServerEmail(QObject *parent) : QObject(parent)
{
  logging.setLogFile("/home/alexandr/mutt/log.log");
  set_folder = "/home/alexandr/mutt/tasks/";
  test_mail = "rburdin@mail.ru";
  server_is_run = true;
}

void ServerEmail::reqestServer(QString reqest, int descriptor)
{
  qDebug() << descriptor;
  cout<< "старт" << std::endl;
  cout<< "reqest" << reqest.toStdString() << std::endl;

  cout<< "is run" << server_is_run << std::endl;
  if (reqest == "ok")
    if (server_is_run)
      serverRun();

}

void ServerEmail::queueAdd(int id)
{
  queue_this.enqueue(id);
}

bool ServerEmail::idTrue(int id)
{
  if (active.at(id)) return true;
  return false;
}

void ServerEmail::toLog(int id)
{
  QString str = "№ письма от провайдера: | %L1 | Провайдер: | %L2 | Емаил: | %L3 | Время отправки: | %L4 |"
                " Ошибка: | %L5 | ";
  str = str.arg(count_list.at(id)).arg(login.at(id)).arg(email_now)
    .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")).arg(out_error);

  QString f = set_folder; f.append(name_folder.at(id)); f.append("error.log");
  QFile file(f); // создаем объект класса QFile

  if (file.open(QIODevice::Append))
    {
      QTextStream out(&file); out.setCodec("UTF-8");
      out << str.append("\n");
    }

  file.close();
  qDebug() << str;
      //  cout << str.toStdString() << std::endl;

  if (count_list.at(id) == per_day.at(id))
    delTask(id);

  if (!out_error.isEmpty())
    delTask(id);
  emit returnServer(str, 1);
}

void ServerEmail::serverRun()
{
  if (server_is_run)
    {
       cout<< "запуск" << std::endl;
      serverConfig();
    //  createQueue();
      cout<< "конфигурация завершена" << std::endl;
      serverStart();
    }
  else server_is_run = true;
}

void ServerEmail::serverConfig()
{
 // cout<< "конфигурация" << std::endl;
  readListFolder();
  readListSet();

}

void ServerEmail::readListFolder()
{
  qDebug()<<"readListFolder()";
  QString str;
  int y;
  for (int i=1; i<59; i++)
    {
      str = QString::number(i);
      y = 4-str.size();
      if ( y < 4)
        {
          for (; y!=0; y--)
            str.prepend("0");
        }
      str.append("/");
      name_folder << str;
    }
  name_folder.removeAt(47);
  name_folder.removeAt(31);
  name_folder.removeAt(30);
  name_folder.removeAt(29);
  name_folder.removeAt(28);
  name_folder.removeAt(27);
  name_folder.removeAt(26);
  name_folder.removeAt(25);
  name_folder.removeAt(24);
  name_folder.removeAt(23);
  name_folder.removeAt(22);
  name_folder.removeAt(21);
  name_folder.removeAt(20);
  name_folder.removeAt(19);
  name_folder.removeAt(18);
  name_folder.removeAt(17);
  name_folder.removeAt(16);
  name_folder.removeAt(14);
  name_folder.removeAt(13);
  name_folder.removeAt(12);
  name_folder.removeAt(11);
  name_folder.removeAt(10);
  name_folder.removeAt(9);
  name_folder.removeAt(8);
  name_folder.removeAt(7);
  name_folder.removeAt(6);
  name_folder.removeAt(5);
  name_folder.removeAt(4);
  name_folder.removeAt(2);
  qDebug()<<"readListFolder()" << name_folder;
}

void ServerEmail::readListSet()
{
  qDebug()<<"readListSet()";
  QStringList buf;
  QString config, buf2;
  for (int i=0; i < name_folder.size(); i++)
    {
      qDebug()<< i;
      config = readConfigFile(name_folder.at(i));
      buf = argumentsSet(config, i);
      arguments.append(buf);
      buf2 = messageSet(config, i);
      message.append(buf2);
      buf2 = nameOrganisationSet(config);
      nameOrganisation.append(buf2);

      per_day.append(perDaySet(config));
      timer.append(timerSet(config));
      active.append(activeSet(config));

      loginPassSet(config);
    }
}

void ServerEmail::loginPassSet(QString argum)
{
  QStringList list1 = argum.split("|");
  login << list1.at(2);
  pass << list1.at(3);
}


void ServerEmail::serverStart()
{
  QString str;
  if (server_is_run)
    {
      for (int i=0; i<login.size();i++)
        {
          cout<< "Отправка" << std::endl;
          transportMail(i);
          Sleep::msleep(60*5*1000/login.size());
          logging.addLog(QString::number(login.size()));
        }
      if (login.size()!=0) serverStart();

      str = "Все задания завершены, запуск построения отчёта | ";
      str.append(QDateTime::currentDateTime().toString("yyyy.MM.dd hh.mm.ss"));
      logging.addLog(str);

     // SubscribeConfig *subscribe = new SubscribeConfig;
     // subscribe->~QObject();

      str = "Построение отчёта завершенно, перезагрузка баз | ";
      str.append(QDateTime::currentDateTime().toString("yyyy.MM.dd hh.mm.ss"));
      logging.addLog(str);

      DBReload *reload = new DBReload;
      reload->replaseDefault();

      awaitIsStart();
    }
  Sleep::msleep(500);
  serverStart();
}

void ServerEmail::transportMail(int id)
{
  switch_provider(id);
  if (idTrue(id))
    {
      mailGo(id);
    }
  else
    serverStart();
}

bool ServerEmail::activeSet(QString argum)
{
  QStringList list1 = argum.split("|");
  QString str = list1.at(11);
  if (str == "1") return true;
  return false;
}

void ServerEmail::mailGo(int id)
{
  QString out, command("mutt");
  QProcess process; process.setProcessChannelMode(QProcess::MergedChannels);
  process.setStandardInputFile(message.at(id)); // Изменить стандартный ввод

  QStringList arg = argumAndMail(id);
  process.start(command, arg);
  process.waitForStarted();
  if (!process.waitForFinished(-1)) out = process.errorString();
  else out = process.readAll();

  countListRefresh(id);
  out_error = out;

  toLog(id);

  process.close();
}

void ServerEmail::switch_provider(int id)
{
  toConfigFile(setProvider(login.at(id), pass.at(id), id));
}

QString ServerEmail::email(int id)
{
  QString mail = emailIsBaseCSV(id);

  email_now = mail;
  QString f = set_folder; f.append(name_folder.at(id)); f.append("email.log");

  QFile file2(f);
  if (file2.open(QIODevice::Append))
    {
      QTextStream out(&file2); out.setCodec("UTF-8");
      out << mail;
    }
  file2.close();

  return email_now;
}

QString ServerEmail::readConfigFile(QString folder)
{
  QString f = set_folder; f.append(folder); f.append("config.conf");
  QFile file(f); // создаем объект класса QFile
  QStringList list;

  if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
      while (!file.atEnd())
        list << file.readLine();
    }
  file.close();
  return list.at(1);
}

QString ServerEmail::messageSet(QString argum, int id)
{
  QStringList list1 = argum.split("|");
  QString str; str.append(set_folder).append(name_folder.at(id))
      .append(list1.at(8));
  return str;
}

QString ServerEmail::nameOrganisationSet(QString argum)
{
  QStringList list1 = argum.split("|");
  return list1.at(0);
}

QStringList ServerEmail::argumentsSet(QString argum, int id)
{
  QStringList list1 = argum.split("|");
  QStringList list2;
  QString str;

  if (list1.at(4) == "none")
    list2 << "-e" << "set content_type=text/html" << "-s" << list1.at(1); // mail
  else
    {
      str = set_folder;
      str.append(name_folder.at(id));
      str.append(list1.at(4));
      list2 << "-e"
            << "set content_type=text/html"
            << "-s"
            << list1.at(1)
            // mail
            << "-a"
            << str;
    }

  return list2;
}

QStringList ServerEmail::argumAndMail(int id)
{
  QStringList list = arguments.at(id);
  if (list.size() == 4)
    list << email(id);
  else
    {
      QStringList listBuf;
      listBuf << list.at(4) << list.at(5);
      list.removeOne(list.last()); list.removeOne(list.last());
      list << email(id) << listBuf.at(0) << listBuf.at(1);
    }
  return list;
}

int ServerEmail::perDaySet(QString argum)
{
  QStringList list1 = argum.split("|");
  return list1.at(9).toInt();
}

int ServerEmail::timerSet(QString argum)
{
  QStringList list1 = argum.split("|");
  return list1.at(10).toInt();
}

void ServerEmail::countListRefresh(int id)
{
  if (count_list.empty())
      count_list << 0;
  int size = count_list.size();
  size = size - id;
  if (size <= 1)
    {
      size = size * (-1) + 1;
      for (int i=0; i<size;i++)
        count_list << 0;
    }
  count_list.replace(id, count_list.at(id)+1);
}

void ServerEmail::delTask(int id)
{
  name_folder.removeAt(id);
  arguments.removeAt(id);
  message.removeAt(id);
  nameOrganisation.removeAt(id);
  login.removeAt(id);
  pass.removeAt(id);
  timer.removeAt(id);
  per_day.removeAt(id);
  count_list.removeAt(id);
  active.removeAt(id);
}

bool ServerEmail::toConfigFile(QStringList list_config)
{
  QString f = "/home/alexandr/.muttrc";
  QFile file(f); // создаем объект класса QFile


  if(file.open(QIODevice::WriteOnly | QIODevice::Text))
  {
      QTextStream out(&file);
      int i = 0;
      while (list_config.size()>i)
      {
          out << list_config.at(i);
          i++;
      }
  }
  file.close();

  emit this->returnServer("Конфигурация файла .muttrc изменена", 1);

  return true;
}

QStringList ServerEmail::setConfigYandexRU(QString email, QString pass, int id)
{
  QString str;
  QStringList list_config;
  str =  "set from = \"EMAIL\"\n^^"
         "set use_from = yes\n^^"
        "set spoolfile = \"imaps://EMAIL:PASS@imap.yandex.ru/INBOX/\"\n^^"
        "set smtp_url = \"smtps://EMAIL@smtp.yandex.ru:465/\"\n^^"
        "set smtp_pass = \"PASS\"\n^^"
        "set realname = \"NAME\"\n^^"
        "set pgp_timeout = 3600\n^^"
        "set ssl_force_tls = yes";
  str.replace("EMAIL", email, Qt::CaseSensitive);
  str.replace("PASS", pass, Qt::CaseSensitive);
  str.replace("NAME", nameOrganisation.at(id), Qt::CaseSensitive);
  list_config = str.split("^^");
  return list_config;
}
QStringList ServerEmail::setConfigBegetCom(QString email, QString pass, int id)
{
  QString str;
  QStringList list_config;
  str =  "set from = \"EMAIL\"\n^^"
         "set use_from = yes\n^^"
        "set spoolfile = \"imaps://EMAIL:PASS@imap.beget.com/INBOX/\"\n^^"
        "set smtp_url = \"smtps://EMAIL@imap.beget.com:465/\"\n^^"
        "set smtp_pass = \"PASS\"\n^^"
        "set realname = \"NAME\"\n^^"
        "set pgp_timeout = 3600\n^^"
        "set ssl_force_tls = yes";
  str.replace("EMAIL", email, Qt::CaseSensitive);
  str.replace("PASS", pass, Qt::CaseSensitive);
  str.replace("NAME", nameOrganisation.at(id), Qt::CaseSensitive);
  list_config = str.split("^^");
  return list_config;
}
QStringList ServerEmail::setConfigRamblerRU(QString email, QString pass, int id)
{
  QString str;
  QStringList list_config;
  str =  "set from = \"EMAIL\"\n^^"
         "set use_from = yes\n^^"
        "set spoolfile = \"imaps://EMAIL:PASS@imap.rambler.ru/INBOX/\"\n^^"
        "set smtp_url = \"smtps://EMAIL@smtp.rambler.ru:465/\"\n^^"
        "set smtp_pass = \"PASS\"\n^^"
        "set realname = \"NAME\"\n^^"
        "set pgp_timeout = 3600\n^^"
        "set ssl_force_tls = yes";
  str.replace("EMAIL", email, Qt::CaseSensitive);
  str.replace("PASS", pass, Qt::CaseSensitive);
  str.replace("NAME", nameOrganisation.at(id), Qt::CaseSensitive);
  list_config = str.split("^^");
  return list_config;
}
QStringList ServerEmail::setConfigMailRU(QString email, QString pass, int id)
{
  QString str;
  QStringList list_config;
  str = "set from = \"EMAIL\"\n^^"
        "set use_from = yes\n^^"
        "set spoolfile = \"imaps://EMAIL:PASS@imap.mail.ru/INBOX/\"\n^^"
        "set smtp_url = \"smtps://EMAIL@smtp.mail.ru:465/\"\n^^"
        "set smtp_pass = \"PASS\"\n^^"
        "set realname = \"NAME\"\n^^"
        "set ssl_force_tls = yes";
   str.replace("EMAIL", email, Qt::CaseSensitive);
   str.replace("PASS", pass, Qt::CaseSensitive);
   str.replace("NAME", nameOrganisation.at(id), Qt::CaseSensitive);
  list_config = str.split("^^");
  return list_config;
}
QStringList ServerEmail::setConfigGmailCom(QString email, QString pass, int id)
{

  QString str;
  QStringList list_config;
  str =  "set from = \"EMAIL\"\n^^"
         "set smtp_url =  \"smtp://EMAIL@smtp.gmail.com:587/\"\n^^"
         "set smtp_pass = \"PASS\"\n^^"
         "set realname = \"NAME\"\n^^"
         "set spoolfile = \"+INBOX\"\n^^"
         "set trash = \"+[Gmail].Spam\"\n^^"
         "set postponed = \"+[Gmail].Drafts\"\n^^"
         "set record = \"+[Gmail].Sent\\ Mail\"\n^^"
         "set header_cache = \"~/.mutt/cache/headers1\"\n^^"
         "set message_cachedir = \"~/.mutt/cache/bodies1\"\n^^"
         "set certificate_file = \"~/.mutt/certificate1\"\n^^"
         "set query_command = \"goobook query '%s'\"\n^^"
         "bind editor <Tab> complete-query\n^^"
         "set pgp_autosign = yes\n^^"
         "set pgp_timeout = 1800\n^^"
         "set pgp_verify_sig = yes\n^^"
         "unset crypt_autosmime\n^^"
         "set pgp_replysign\n^^"
         "set pgp_replyencrypt\n^^"
         "set pgp_replysignencrypted\n^^"
         "set pgp_verify_sig";
  str.replace("EMAIL", email, Qt::CaseSensitive);
  str.replace("PASS", pass, Qt::CaseSensitive);
  str.replace("NAME", nameOrganisation.at(id), Qt::CaseSensitive);
  list_config = str.split("^^");

  return list_config;
}
QStringList ServerEmail::setProvider(QString provider, QString pass, int id)
{
  QStringList l;

  if      (provider.indexOf("yandex.ru")>-1)
    l = setConfigYandexRU(provider, pass, id);
  else if (provider.indexOf("mail.ru")>-1)
    l = setConfigYandexRU(provider, pass, id);
  else if (provider.indexOf("gmail.com")>-1)
    l = setConfigGmailCom(provider, pass, id);
  else if (provider.indexOf("gazeta-naroda.ru")>-1)
    l = setConfigGmailCom(provider, pass, id);
  else if (provider.indexOf("rambler.ru")>-1)
    l = setConfigRamblerRU(provider, pass, id);
  else if (provider.indexOf("lenta.ru")>-1)
    l = setConfigRamblerRU(provider, pass, id);
  else if (provider.indexOf("training-tlt.ru")>-1)
    l = setConfigBegetCom(provider, pass, id);


  if (l.size()==0) l << "";

  return l;
}

bool ServerEmail::mailFilter(QString mail, int id)
{
  QString provider = login.at(id);

  if (provider.contains("poll.dekor@gmail.com"))
    {
      if (mail.contains("@mail.ru")) return 0;
      if (mail.contains("@bk.ru")) return 0;
      if (mail.contains("@list.ru")) return 0;
    }
  if (provider.contains("tosdoma.3.16.pobeda@gmail.com"))
    {
      if (mail.contains("@mail.ru")) return 0;
      if (mail.contains("@bk.ru")) return 0;
      if (mail.contains("@list.ru")) return 0;
    }
  return 1;
}

void ServerEmail::TESTreqestServer(QString reqest, int descriptor)
{
  qDebug() << descriptor;
  cout<< "старт" << std::endl;
  cout<< "reqest" << reqest.toStdString() << std::endl;

  cout<< "is run" << server_is_run << std::endl;
  if (reqest == "ok")
    if (server_is_run)
      TESTserverRun();

}
void ServerEmail::TESTserverRun()
{
  if (server_is_run)
    {
       cout<< "запуск" << std::endl;
      TESTserverConfig();
    //  createQueue();
      cout<< "конфигурация завершена" << std::endl;
      TESTserverStart();
    }
  else server_is_run = true;
}
void ServerEmail::TESTserverConfig()
{
  cout<< "конфигурация" << std::endl;
  TESTreadListFolder();
  TESTreadListSet();

}
void ServerEmail::TESTreadListFolder()
{
  name_folder.clear();
  name_folder << "0002/";
  name_folder << "0004/";
  name_folder << "0012__/";
  name_folder << "0016/";
}
void ServerEmail::TESTreadListSet()
{
  QStringList buf;
  QString config, buf2;
  for (int i=0; i < name_folder.size(); i++)
    {
      config = readConfigFile(name_folder.at(i));
      buf = TESTargumentsSet(config, i);
      arguments.append(buf);
      buf2 = messageSet(config, i);
      message.append(buf2);
      buf2 = nameOrganisationSet(config);
      nameOrganisation.append(buf2);

      per_day.append(perDaySet(config));
      timer.append(timerSet(config));
      active.append(activeSet(config));

      loginPassSet(config);
    }
}
QStringList ServerEmail::TESTargumentsSet(QString argum, int id)
{
  QStringList list1 = argum.split("|");
  QStringList list2;
  QString str;

  if (list1.at(4) == "none")
    {
      list2 << "-e" << "set content_type=text/html" << "-s"
            << list1.at(1)
            << test_mail
            << "-c"
            << "gg.shaman888@gmail.com";

    }
  else
    {
      str = set_folder;
      str.append(name_folder.at(id));
      str.append(list1.at(4));
      list2 << "-e"
            << "set content_type=text/html"
            << "-s"
            << list1.at(1)
            << test_mail
            << "-c"
            << "gg.shaman888@gmail.com"
            << "-a"
            << str;
    }

  qDebug () << list2;
  return list2;
}
void ServerEmail::TESTserverStart()
{
  if (server_is_run)
    {
      for (int i=0; i<login.size();i++)
        {
          cout<< "Отправка" << std::endl;
          TESTtransportMail(i);
          //Sleep::msleep(60*5*1000/login.size());
        }
      if (login.size()!=0) {
          Sleep::msleep(500000);
          TESTserverStart();

        }
      qDebug() << "Все задания завершены";
      cout<< "Все задания завершены" << std::endl;
      Sleep::msleep(500000);
    }
  Sleep::msleep(500);
  TESTserverStart();
}
void ServerEmail::TESTtransportMail(int id)
{
  switch_provider(id);
  if (idTrue(id))
    {
      TESTmailGo(id);
    }
  else
    serverStart();
}
void ServerEmail::TESTmailGo(int id)
{
  QString out, command("mutt");
  QProcess process; process.setProcessChannelMode(QProcess::MergedChannels);
  process.setStandardInputFile(message.at(id)); // Изменить стандартный ввод

  QStringList arg = arguments.at(id);
  process.start(command, arg);
  process.waitForStarted();
  if (!process.waitForFinished(-1)) out = process.errorString();
  else out = process.readAll();

  countListRefresh(id);
  out_error = out;

  //toLog(id);

  process.close();
}

QString ServerEmail::emailIsBaseCSV(int id)
{
  QString path_file = pathSubscribeBase(id);

  QFile file(path_file); // создаем объект класса QFile
  QStringList list;

  if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
      while (!file.atEnd())
        list << file.readLine();
    }
  file.close();

  QString mail;

  if (id==1)
    qDebug() << "";

  if(!list.isEmpty())
    {
      for(int i=list.size()-1; i>-1; i--)
        {
          mail = list.at(i);
          if (mailFilter(mail, id))
            break;
        }
      list.removeAll(mail);

      if (file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
          QTextStream out(&file); out.setCodec("UTF-8");
          for (int i=0; i<list.size();i++)
            out << list.at(i);
        }

      file.close();
    }

  mail.remove(10);
  return mail;
}

void ServerEmail::awaitIsStart()
{
  int time = startIsTime();
  if (time<0)
    {
      TESTreqestServer("ok", 1);
      reqestServer("ok", 1);
    }

  Sleep::msleep(time);
  TESTreqestServer("ok", 1);
  reqestServer("ok", 1);

}

int ServerEmail::startIsTime()
{
  TaskConfig *task = new TaskConfig;
  task->readConfigTasks("/home/alexandr/mutt/tasks/");
  readListFolder();

  return task->isTimeStart(name_folder);
}

QString ServerEmail::pathSubscribeBase(int id)
{
  QString name = name_folder.at(id);
  name.resize(name.size()-1);

  if (name[0]=='0') name.remove(0, 1);
  if (name[0]=='0') name.remove(0, 1);
  if (name[0]=='0') name.remove(0, 1);

  int x = name.toInt();

  if (x>0 and x<4) return   "/home/alexandr/mutt/subscribe/0001/base.csv";
  if (x>3 and x<11) return  "/home/alexandr/mutt/subscribe/0002/base.csv";
  if (x>18 and x<23) return "/home/alexandr/mutt/subscribe/0002/base.csv";
  if (x>45 and x<56) return "/home/alexandr/mutt/subscribe/0002/base.csv";
  if (x>10 and x<15) return "/home/alexandr/mutt/subscribe/0003/base.csv";
  if (x>22 and x<33) return "/home/alexandr/mutt/subscribe/0003/base.csv";
  if (x>14 and x<19) return "/home/alexandr/mutt/subscribe/0004/base.csv";
  if (x>32 and x<46) return "/home/alexandr/mutt/subscribe/0004/base.csv";
  if (x>55 and x<59) return "/home/alexandr/mutt/subscribe/0005/base.csv";

  return "";
}
