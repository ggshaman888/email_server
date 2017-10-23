#include "config/muttconfig.h"
#include <QFile>
#include <QTextCodec>
#include <QTextStream>

MuttConfig::MuttConfig(QObject *parent) : QObject(parent)
{
path_mutt_config = "/home/alexandr/.muttrc";
}

void MuttConfig::setPathMuttConfig(QString file)
{
  path_mutt_config = file;
}

void MuttConfig::setProvider(QString name)
{
  provider = name;
}

void MuttConfig::setPass(QString password)
{
  pass = password;;
}

void MuttConfig::setMail(QString email)
{
  mail = email;
}

void MuttConfig::setName(QString name_set)
{
  name = name_set;
}

void MuttConfig::setTheme(QString name)
{
  theme = name;
}

void MuttConfig::setCSender(QString sender)
{
  c_sender << sender;
}

void MuttConfig::setAttach(QString file)
{
  attach << file;
}

void MuttConfig::setMessage(QString file)
{
  message = file;
}

QStringList MuttConfig::isArguments()
{
  argument.clear();
  if (configAccess())
    {
      argument  << "-e"
                << "set content_type=text/html"
                << "-s"
                << theme
                << mail;
      if(!c_sender.isEmpty())
        for(int i=0;i<c_sender.size();i++)
          argument << "-c" << c_sender.at(i);

      if(!attach.isEmpty())
        for(int i=0;i<attach.size();i++)
          argument << "-a" << attach.at(i);

      return argument;
    }
  argument.clear();
  return argument;
}

QString MuttConfig::isMessage()
{
  return message;
}

bool MuttConfig::switchConfig()
{
  if (configAccess())
    {
      setConfig();
      replaseConfig();
      return true;
    }
  return false;
}

bool MuttConfig::configAccess()
{
  if (!provider.isEmpty())
    if (!pass.isEmpty())
      if (!mail.isEmpty())
        if(!theme.isEmpty())
          if(!name.isEmpty())
            return true;

  return false;
}

void MuttConfig::setConfigYandexRU()
{
  QString str =  "set from = \"EMAIL\"\n^^"
         "set use_from = yes\n^^"
        "set spoolfile = \"imaps://EMAIL:PASS@imap.yandex.ru/INBOX/\"\n^^"
        "set smtp_url = \"smtps://EMAIL@smtp.yandex.ru:465/\"\n^^"
        "set smtp_pass = \"PASS\"\n^^"
        "set realname = \"NAME\"\n^^"
        "set pgp_timeout = 3600\n^^"
        "set ssl_force_tls = yes";
  str.replace("EMAIL", provider, Qt::CaseSensitive);
  str.replace("PASS", pass, Qt::CaseSensitive);
  str.replace("NAME", name, Qt::CaseSensitive);
  list_config = str.split("^^");
}

void MuttConfig::replaseConfig()
{
  QFile file(path_mutt_config); // создаем объект класса QFile

  if(file.open(QIODevice::WriteOnly | QIODevice::Text))
  {
      QTextStream out(&file);
      for(int i=0;i<list_config.size();i++)
        out << list_config.at(i);
  }
  file.close();
}
void MuttConfig::setConfigRamblerRU()
{
  QString str =  "set from = \"EMAIL\"\n^^"
         "set use_from = yes\n^^"
        "set spoolfile = \"imaps://EMAIL:PASS@imap.rambler.ru/INBOX/\"\n^^"
        "set smtp_url = \"smtps://EMAIL@smtp.rambler.ru:465/\"\n^^"
        "set smtp_pass = \"PASS\"\n^^"
        "set realname = \"NAME\"\n^^"
        "set pgp_timeout = 3600\n^^"
        "set ssl_force_tls = yes";
  str.replace("EMAIL", provider, Qt::CaseSensitive);
  str.replace("PASS", pass, Qt::CaseSensitive);
  str.replace("NAME", name, Qt::CaseSensitive);
  list_config = str.split("^^");
}
void MuttConfig::setConfigMailRU()
{
  QString str = "set from = \"EMAIL\"\n^^"
        "set use_from = yes\n^^"
        "set spoolfile = \"imaps://EMAIL:PASS@imap.mail.ru/INBOX/\"\n^^"
        "set smtp_url = \"smtps://EMAIL@smtp.mail.ru:465/\"\n^^"
        "set smtp_pass = \"PASS\"\n^^"
        "set realname = \"NAME\"\n^^"
        "set ssl_force_tls = yes";
   str.replace("EMAIL", provider, Qt::CaseSensitive);
   str.replace("PASS", pass, Qt::CaseSensitive);
   str.replace("NAME", name, Qt::CaseSensitive);
  list_config = str.split("^^");
}
void MuttConfig::setConfigGmailCom()
{
  QString str =  "set from = \"EMAIL\"\n^^"
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
         "set pgp_timeout = 3600\n^^"
         "set pgp_verify_sig = yes\n^^"
         "unset crypt_autosmime\n^^"
         "set pgp_replysign\n^^"
         "set pgp_replyencrypt\n^^"
         "set pgp_replysignencrypted\n^^"
         "set pgp_verify_sig\n^^"
         "set ssl\\_force\\_tls = yes";
  str.replace("EMAIL", provider, Qt::CaseSensitive);
  str.replace("PASS", pass, Qt::CaseSensitive);
  str.replace("NAME", name, Qt::CaseSensitive);
  list_config = str.split("^^");
}
void MuttConfig::setConfig()
{
  if      (provider.indexOf("yandex.ru")>-1) setConfigYandexRU();
  else if (provider.indexOf("mail.ru")>-1) setConfigMailRU();
  else if (provider.indexOf("bk.ru")>-1) setConfigMailRU();
  else if (provider.indexOf("list.ru")>-1) setConfigMailRU();
  else if (provider.indexOf("inbox.ru")>-1) setConfigMailRU();
  else if (provider.indexOf("gmail.com")>-1) setConfigGmailCom();
  else if (provider.indexOf("gazeta-naroda.ru")>-1) setConfigGmailCom();
  else if (provider.indexOf("rambler.ru")>-1) setConfigRamblerRU();
  else if (provider.indexOf("lenta.ru")>-1) setConfigRamblerRU();
}
