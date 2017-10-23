#ifndef SERVEREMAIL_H
#define SERVEREMAIL_H

#include <QObject>
#include <QQueue>
#include <QMap>
#include <QStringList>
#include "log.h"

class ServerEmail : public QObject
{
  Q_OBJECT
public:
  explicit ServerEmail(QObject *parent = 0);
  void TESTreqestServer(QString, int);

signals:
  void switchProvider(QString, QString);
  void transport (QString, QString);
  void setFolderSig(QString);

  void returnServer(QString, int);

public slots:
  void reqestServer(QString, int);
  void queueAdd(int);
/*
  void setFolder(QString);
  void transportReturn(QString);
  void setConfig(QString);
*/
private:
  bool server_is_run;
  bool idTrue(int);
  void toLog(int);
  void serverRun();
  void serverConfig();
  void readListFolder();
  void readListSet();
  void loginPassSet(QString);
//  void createQueue();
  void serverStart();
  void transportMail(int);
  bool activeSet(QString);
  void mailGo(int);
  void switch_provider(int);
  QString email(int);
  QString readConfigFile(QString);
  QString messageSet(QString, int);
  QString nameOrganisationSet(QString);
  QStringList argumentsSet(QString, int);
  QStringList argumAndMail(int);
  int perDaySet(QString);
  int timerSet(QString);
  void countListRefresh(int);
  void delTask(int);
  bool toConfigFile(QStringList);
  QStringList setConfigYandexRU(QString, QString, int);
  QStringList setConfigBegetCom(QString, QString, int);
  QStringList setConfigRamblerRU(QString, QString, int);
  QStringList setConfigMailRU(QString, QString, int);
  QStringList setConfigGmailCom(QString, QString, int);
  QStringList setProvider(QString, QString, int);

  QString set_folder;
  QStringList name_folder;
  QList <QStringList> arguments;
  QStringList message;
  QStringList nameOrganisation;
  QStringList login;
  QStringList pass;
  QList <int> timer;
  QList <int> per_day;
  QList <int> count_list;
  QList <bool> active;
  QQueue <int> queue_this;

  QString email_now;
  QString out_error;

  bool mailFilter(QString, int);


  void TESTserverRun();
  void TESTserverConfig();
  void TESTreadListFolder();
  void TESTreadListSet();
  QStringList TESTargumentsSet(QString, int);
  void TESTserverStart();
  void TESTtransportMail(int);
  QString test_mail;
  void TESTmailGo(int);
  QString emailIsBaseCSV(int);

  void awaitIsStart();
  int startIsTime();

  QString pathSubscribeBase(int);

  Log logging;

};

#endif // SERVEREMAIL_H
