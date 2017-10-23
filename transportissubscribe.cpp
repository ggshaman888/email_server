#include "transportissubscribe.h"
/*
TransportIsSubscribe::TransportIsSubscribe(QObject *parent) : QObject(parent)
{

}

void TransportIsSubscribe::readPathToSubscribes(QString file)
{
  path_to_subscribes = file;
}

void TransportIsSubscribe::runTransportMail()
{
  readActiveSubscribe();
  startTransport();
}

void TransportIsSubscribe::startTransport()
{
  for(int subscribe_number=0; i<active_subscribe.size();subscribe_number++)
    {
      for(int mail_number=0;i<active_subscribe.at(i);mail_number++)
        {
          if (activeProvider(subscribe_number, mail_number))
            {
              setMessage(subscribe_number, mail_number);
              transportMail();
              setResult(subscribe_number, mail_number);
              output(subscribe_number, mail_number);
            }
        }
    }
  if (!active_subscribe.isEmpty())
    startTransport();
}

void TransportIsSubscribe::setMessage(int subscribe_number, int mail_number)
{
  setPathSubscribe(subscribe_number);
  readMailProvider(mail_number);
  setArguments();
}

void TransportIsSubscribe::transportMail()
{
  QString out, command("mutt");
  QProcess process; process.setProcessChannelMode(QProcess::MergedChannels);
  process.setStandardInputFile(message); // Изменить стандартный ввод

  process.start(command, arguments);
  process.waitForStarted();

  if (!process.waitForFinished(-1)) out_error = process.errorString();
  else out_error = process.readAll();

  process.close();
}

void TransportIsSubscribe::setResult(int subscribe_number, int mail_number)
{
  // общий счётчик

  // записать счётчик провайдера с ошибкой или без

  // записать событие в лог

  // если ошибка поменять бит активности провайдера

}

void TransportIsSubscribe::readActiveSubscribe()
{

}

*/
