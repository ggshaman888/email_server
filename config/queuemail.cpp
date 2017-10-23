#include "queuemail.h"
#include <config/subscribeconf.h>
#include <config/configprovider.h>
#include <config/muttconfig.h>

QueueMail::QueueMail(QObject *parent) : QObject(parent)
{
  range = 1440;
  path_subscribe = "/home/alexandr/mutt/subscribe";
}

void QueueMail::push(QString sub_folder, QString provider)
{
  if(!subscribe.contains(sub_folder))
    subscribe << sub_folder;

  this_subscribe = sub_folder;

  distribute(provider);
}

void QueueMail::distribute(QString provider)
{
  int c_left = countLeft();
  int c_provider = countProvider(provider);

  int remainder = c_left - c_provider;

  if(remainder > c_provider)
    {
      int interval = range/c_provider;
      for(int i=1; i<c_provider+1; i++)
        addQueue(provider, interval*i);
    }
  else
    {
      int interval = range/remainder;
      for(int i=1; i<remainder+1; i++)
        addQueue(provider, interval*i);
    }
}

int QueueMail::countLeft()
{
  int x = subscribe.indexOf(this_subscribe);
  return max_count.at(x)-added_count.at(x);
}

int QueueMail::countProvider(QString provider)
{
  ConfigProvider *p = new ConfigProvider;
  p->setPath(QString(path_subscribe +"/" + this_subscribe + "/providers.conf"));

  if(p->setIterProvider(provider))
    {
      int x = p->per_day_recomendation - p->posted_today;
      if(x<1)
        return 0;
      else
        return x;
    }
  return 0;
}

void QueueMail::addQueue(QString provider, int minutes)
{
  QMap<int, QList<QStringList>>::const_iterator iter = queue.begin();
/*  while(!queue.last())
    {
      iter.key();
      queue.lowerBound(provider);
    }*/
}

/*
void QueueMail::setSubscribePath(QString path_subscribe)
{
  ConfigProvider *p = new ConfigProvider;

  SubscribeConf *s = new SubscribeConf;
  s->setPath(path_subscribe);
  while(s->nextActive())
    {
      s->message_path;
      s->per_day;
      s->posted_today;
      s->interval;
      s->isFolder();
      s->name_sender;
      // provider|max_per_day-count_per_day

      QmapProviderPul(p->setPath(path_subscribe.append("/").append(s->isFolder())));
      maxPulProvider();
      s->isMail();
    }
}

void QueueMail::push(SubscribeConf sub)
{
  int max_prov = max_count_provider(sub.path_subscribes);
  QList<int> count_balanse = countBalanse(max_prov, );
  provider + datetime + ms +
  // узнать сколько можно оправить провайдеру исходя из возможностей провайдеров
  // распределить нагрузку между провайдерами
  // распределить время отправления на весь день
  // при добавлении в одно время добалять милисекунды





  prov.per_day_recomendation;
  prov.posted_today;
  prov.provider;
  prov.pas;
  prov.theme;

}
*/
