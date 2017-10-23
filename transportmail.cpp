#include "transportmail.h"
#include "subscribeconfig.h"
#include "config/muttconfig.h"
#include <QProcess>
#include <QStringList>
#include <QDebug>
#include <sleep.h>

TransportMail::TransportMail(QObject *parent) : QObject(parent)
{

}

void TransportMail::startDefault()
{
  SubscribeConfig *sub = new SubscribeConfig;
  MuttConfig *mutt = new MuttConfig;

  SubscribeConfig::ConfigIs conf;
  QString path_sub = "/home/alexandr/mutt/subscribe/";
  QString error_out, str;
  QStringList l;

  Sleep *sl = new Sleep;


  sub->setPathSubscribe(path_sub);
  int i = 0;
  bool ok = sub->next();
  while (ok)
    {
      conf = sub->isSubscribe();
      mutt->setMail(sub->isMail());
      mutt->setName(conf.name_sender);
      mutt->setProvider(sub->isProvider().provider);
      mutt->setPass(sub->isProvider().pass);
      mutt->setTheme(sub->isSubscribe().theme);
      mutt->switchConfig();

      str = path_sub; str.append(sub->isSubscribe().message_path);
      error_out = transport(mutt->isArguments(), str);

      sub->updateLog(error_out);

      if(error_out.isEmpty())
        i++;

      qDebug() << i << mutt->mail << sub->isProvider().provider << error_out.remove("\\n")
               << QDateTime::currentDateTime().time();
      sl->msleep(200);
      sl->msleep(60*5*1000/sub->allSubscribe().size());

      if(isTimeOut())
        break;

      ok = sub->nextSet();
    }
  qDebug() << "end";
  sl->msleep(200000);
  startDefault();
}

QString TransportMail::transport(QStringList argument, QString message)
{
  QString out, command("mutt");
  QProcess process; process.setProcessChannelMode(QProcess::MergedChannels);
  process.setStandardInputFile(message); // Изменить стандартный ввод

  process.start(command, argument);
  process.waitForStarted();
  if (!process.waitForFinished(-1)) out = process.errorString();
  else out = process.readAll();

  process.close();

  return out;
}

bool TransportMail::isTimeOut()
{
  if(QDateTime::currentDateTime().toString("hh") == "01")
    return true;
  return false;
}

