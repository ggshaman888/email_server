#include <QFile>
#include <QString>
#include <QStringList>
//#include <QSql>
//#include <QSqlQuery>
//#include <QSqlError>
//#include <QSqlRecord>
//#include <QSqlDatabase>
#include <QTextCodec>
#include <QDebug>
#include <QDateTime>
#include <QProcess>
#include "providersmtp.h"
#include "emailedit.h"

ProviderSMTP::ProviderSMTP(QObject *parent) : QObject(parent)
{
  name = "VIN Engeneering";
}

QString ProviderSMTP::nowProvider()
{
  // нужно реализовать разное место хранение данных конфигурационного файла

  QString f = "/home/alexandr/.muttrc";
  QFile file(f); // создаем объект класса QFile
  QStringList list;

  if (existFile(f))
    {
      if(file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
          list << file.readLine();
          f = identification_provider(list);
        }
      file.close();

      if (f.isEmpty())
        emit this->errorSwitch("Провайдер не обнаружен в файле конфигурации");
    }
  else
    emit this->errorSwitch("Провайдер не обнаружен в файле конфигурации");

  return f;
}
/*
void ProviderSMTP::initProvider()
{
  closeProvider();
  QStringList provider = showAllFreeProvider();
  switchProvider(provider.at(0), provider.at(1));
}
*/
void ProviderSMTP::nowProviderGo()
{
  EmailEdit e;
  QString provider = nowProvider();
  if (!e.emailTrue(provider))
    {
      switchProvider("", "");
      provider = nowProvider();
    }

  emit this->nowProviderGo(provider);
}
/*
QStringList ProviderSMTP::showAllFreeProvider()
{
  Randomize r;
  QString connectionName = r.go();
  CreateConnection c; c.createConnectionToThread(connectionName);
  QSqlDatabase db = QSqlDatabase::database(connectionName);
  QSqlQuery query_mysql(db);
  QTextCodec *codec = QTextCodec::codecForName("UTF-8");
  QTextCodec::setCodecForLocale(codec);
  qDebug() << query_mysql.exec("SELECT email, pass, time FROM mail_provider ORDER BY id;");
  // Считываем данные из базы
  qDebug () << query_mysql.lastError().text();

  QStringList option;
  QString mail;
  QDateTime time;

  QSqlRecord rec  = query_mysql.record();
  while (query_mysql.next())
  {
    QSqlRecord rec  = query_mysql.record(); // сравниваем разницу во времени, если провайдер был использован

    mail = query_mysql.value(rec.indexOf("email")).toString();
    time = query_mysql.value(rec.indexOf("time")).toDateTime();

    qDebug() << mail << time;

    if (compareToCurrentDataTime_hours(time, 12) or time.isNull())
    {
        option << query_mysql.value(rec.indexOf("email")).toString();
        option << query_mysql.value(rec.indexOf("pass")).toString();
    }
  }
  if (option.size()==0)
    {
      qDebug() << "Доступных провайдеров не осталось";
      option << "" << ""; // возможно тут ошибка
      emit errorSwitch("Доступных провайдеров не осталось");
      return option;
    }
  QString strf = "UPDATE mail_provider SET activity = '%1' WHERE email = '%2';";
  QString str = strf.arg("2").arg(option.at(0));
  qDebug() << query_mysql.exec(str);

  db.close();
  return option;
}
*/
bool ProviderSMTP::compareToCurrentDataTime_hours (QDateTime time, int difference)
{
  qint64 y;

  y = QDateTime::currentDateTime().secsTo(time);
  if (y > 0) return true;

  difference = difference * 3600;
  if (difference > time.secsTo(QDateTime::currentDateTime())) return false;
  else return true;
}
/*
void ProviderSMTP::closeProvider()
{
  Randomize r;
  QString connectionName = r.go();
  CreateConnection c; c.createConnectionToThread(connectionName);
  QSqlDatabase db = QSqlDatabase::database(connectionName);
  QSqlQuery query_mysql(db);
  QTextCodec *codec = QTextCodec::codecForName("UTF-8");
  QTextCodec::setCodecForLocale(codec);
  QString strf, str;
  strf = "UPDATE mail_provider SET time ='%1', activity = '2' WHERE email = '%2';";
  str = strf.arg(QDateTime::currentDateTime().toString(dataFormat)).arg(nowProvider());
  qDebug() << "провайдер закрыт" << query_mysql.exec(str);
  db.close();
}
*/
QString ProviderSMTP::switchProvider(QString provider, QString pass)
{
  EmailEdit *e = new EmailEdit;

  //closeProvider();

  if (e->emailTrue(provider))
    toConfigFile(setProvider(provider, pass));
  else
    Sleep::msleep(10000000);

    //toConfigFile(setProvider(showAllFreeProvider()));

  return "";
}

bool ProviderSMTP::existFile(QString file)
{
  QFile f(file);
  if (QFile::exists(file) == false)   // проверка на существование
  {
      qDebug () << "Файл отсутствует. Создаём файл конфигурации";
      f.setFileName(file); // создали файл
      if (QFile::exists(file) == false) qDebug () << "error(3)";
      return false;
  }
  return true;
}

QStringList ProviderSMTP::setProvider(QString provider, QString pass)
{
  QStringList l;

  if      (provider.indexOf("yandex.ru")>-1)
    l = setConfigYandexRU(provider, pass);
  else if (provider.indexOf("mail.ru")>-1)
    l = setConfigYandexRU(provider, pass);
  else if (provider.indexOf("gmail.com")>-1)
    l = setConfigGmailCom(provider, pass);

  qDebug () << "\nПровайдер:" << provider;
  if (l.size()==0) l << "";

  return l;
}

QStringList ProviderSMTP::setProvider(QStringList list)
{
  return this->setProvider(list.at(0), list.at(1));
}

QStringList ProviderSMTP::setConfigYandexRU(QString email, QString pass)
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
  str.replace("NAME", name, Qt::CaseSensitive);
  list_config = str.split("^^");
  return list_config;
}

QStringList ProviderSMTP::setConfigMailRU(QString email, QString pass)
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
  str.replace("NAME", name, Qt::CaseSensitive);
  list_config = str.split("^^");
  return list_config;
}

QStringList ProviderSMTP::setConfigGmailCom(QString email, QString pass)
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
  str.replace("NAME", name, Qt::CaseSensitive);
  list_config = str.split("^^");

  return list_config;
}

bool ProviderSMTP::toConfigFile(QStringList list_config)
{
  QString f = "/home/alexandr/.muttrc";
  QFile file(f); // создаем объект класса QFile

  existFile(f); // если не существует, создаём чистый файл

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

  emit this->switchFreeProvider("Конфигурация файла .muttrc изменена");

  return true;
}

QString ProviderSMTP::identification_provider (QStringList line)
{
  if      (line.at(0).indexOf("mail.ru") != -1)   {return email_provider(line.at(0));}
  else if (line.at(0).indexOf("yandex.ru") != -1) {return email_provider(line.at(0));}
  else if (line.at(0).indexOf("gmail.com") != -1) {return email_provider(line.at(0));}
  return "";
}

QString ProviderSMTP::email_provider(QString provider)
{
  QString x = provider.mid(12, provider.size()-14);
  return x;
}


void ProviderSMTP::switchProviderSlot(QString provider, QString pass)
{
  switchProvider(provider, pass);
}

void ProviderSMTP::transport(QString file, QStringList arg)
{
  QString out, command("mutt");
  QProcess process; process.setProcessChannelMode(QProcess::MergedChannels);
  process.setStandardInputFile(file); // Изменить стандартный ввод

  process.start(command, arg);  process.waitForStarted();
  if (!process.waitForFinished(-1)) out = process.errorString();
  else out = process.readAll();

  process.close();
  emit errorTransport(out);
}

