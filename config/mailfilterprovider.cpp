#include "config/mailfilterprovider.h"
#include <QRegExp>
MailFilterProvider::MailFilterProvider(QObject *parent) : QObject(parent)
{

}

bool MailFilterProvider::mailFilterProvider(QString mail, QString provider)
{
  if (mail.contains("-"))
    if(provider.contains("@bk.ru"))
      return 1;
  else
      return 0;

  if (provider.contains("@gmail.com"))
    if (mail.contains("-"))
      return 0;
  if (provider.contains("@yandex."))
    if (mail.contains("-"))
      return 0;

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
  if (provider.contains("vin.test.tlt@gmail.com"))
    {
      if (mail.contains("@mail.ru")) return 0;
      if (mail.contains("@bk.ru")) return 0;
      if (mail.contains("@list.ru")) return 0;
      if (mail.contains("@inbox.ru")) return 0;
    }
  QRegExp rx("[^a-zA-Z\\.\\-@0-9_]");

  if(rx.indexIn(mail)!=-1)
    return 0;

  return 1;
}
