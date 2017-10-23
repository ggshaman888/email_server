#ifndef PROVIDERSMTP_H
#define PROVIDERSMTP_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QThread>

class ProviderSMTP : public QObject
{
  Q_OBJECT
public:
  explicit ProviderSMTP(QObject *parent = 0);

  // реализованно
  QString nowProvider();  // текущий провайдер
  QString switchProvider(QString, QString);
  QStringList showAllFreeProvider();

  QString name;
/*
  // не реализованно
  QString showNextProvider();
  QString showPrevProvider();
  QString showNextAnyProvider();
  QStringList showAllProvider();
*/
signals:
  void switchFreeProvider(QString);
  void errorSwitch(QString);
  void nowProviderGo(QString);

  void errorTransport(QString);

public slots:
 // void initProvider(); // установка свободного провайдера
  void nowProviderGo(); // передать текущего провайдера

  void switchProviderSlot(QString, QString);
  void transport(QString, QStringList);

private:
  QString email_provider(QString);
  QString identification_provider (QStringList);
  bool existFile(QString);
  QStringList setProvider (QString, QString);
  QStringList setProvider (QStringList);
  QStringList setConfigYandexRU (QString, QString);
  QStringList setConfigMailRU (QString, QString);
  QStringList setConfigGmailCom (QString, QString);
  bool toConfigFile(QStringList);
  bool compareToCurrentDataTime_hours (QDateTime, int);
  void   closeProvider();



};

class Sleep: public QThread {
public:
    static void msleep(int ms)
    {
        QThread::msleep(ms);
    }
};
#endif // PROVIDERSMTP_H
