#ifndef SUBSCRIBECONFIG_H
#define SUBSCRIBECONFIG_H

#include <QObject>
#include <QDateTime>
#include <QStringList>
#include "taskconfig.h"

class SubscribeConfig : public QObject
{
  Q_OBJECT
public:
  explicit SubscribeConfig(QObject *parent = 0);

  class ConfigProvider
  {
    public:
      QString provider;
      QString pass;
      int max_count;
      int min_count_to_error;
      int error_count;
      int per_day_recomendation;
      int posted_today;
      int error_today;
      QDateTime last_datatime;
      bool active;
  };

  class ConfigIs
  {
    public:
      QString name_sender;
      QString theme;
      QString attach;
      QString base_name;
      QString mail_log;
      QString error_log;
      QString message_path;
      int per_day;
      int interval;
      bool active;
  };

  int all_active_provider;

  void setPathSubscribe(QString);
  void setActiveSubscribe();
  void setProvider(int);

  bool nextActiveProvider();
  bool nextActiveSubscribe();

  bool nextSet(); // любая следующая конфигурация доступная для запуска. Бесконечный цикл

  bool next();

  QList<int> allSubscribe();
  QList<ConfigProvider> providersSubscribe(int);

//  void setSubscribeActiveTrue(int);
//  void setSubscribeActiveFalse(int);

  QString isMail();
  ConfigProvider isProvider();
  ConfigIs isSubscribe();

  void updateLog(QString); //error

  bool errorCriticalMessage(QString);
  QString numberToFolder(int);
signals:

public slots:

private:

  ConfigIs this_subscribe;
  QString path_subscribes;
  QString mail;
  QList<int> active_subscribe;
  QList<int> all_subscribe;
  QList<ConfigProvider> provider_all;
  QString last_provider;

  int carriage_active_subscribe;
  int carriage_active_provider;

  int folderToNumber(QString);
  bool activeSubscribe(int);
  void mailReset();
  void toLog(QString);
  void providerUpdateStatistic(QString);
  bool isToday(QDateTime);
  void providerReplase(ConfigProvider);
  bool mailFilterProvider(QString);
  void mailBad();
  QStringList removeBadMail(QStringList);

  void toProviderNoActive(QString, QString);
  void setAllActiveProvider();

  ConfigProvider setProviderElements(ConfigProvider);
  QString date_format;
  QString date_time_format;


  bool activeProvider();
  void setActiveSubscribeNew();
  bool setProviderNew(int);
  bool nextActiveProviderNew();
  bool mailRuProvider(QString);

};

#endif // SUBSCRIBECONFIG_H
