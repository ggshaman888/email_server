#ifndef MUTTCONFIG_H
#define MUTTCONFIG_H

#include <QObject>
#include <QStringList>
#include <subscribeconfig.h>

class MuttConfig : public QObject
{
  Q_OBJECT
public:
  explicit MuttConfig(QObject *parent = 0);

  void setPathMuttConfig(QString);
  void setProvider(QString);
  void setPass(QString);
  void setMail(QString);
  void setName(QString);
  void setTheme(QString);
  void setCSender(QString);
  void setAttach(QString);
  void setMessage(QString);

  QStringList isArguments();
  QString isMessage();

  bool switchConfig();

  QStringList argument;
  QString provider;
  QString pass;
  QString mail;
  QString name;
  QString theme;
  QStringList c_sender;
  QStringList attach;

  QStringList list_config;
signals:

public slots:

private:
  QString path_mutt_config;
  QString message;



  bool configAccess();

  void setConfigGmailCom();
  void setConfigMailRU();
  void setConfigRamblerRU();
  void setConfigYandexRU();
  void setConfig();

  void replaseConfig();

};

#endif // MUTTCONFIG_H
