#ifndef SUBSCRIBECONF_H
#define SUBSCRIBECONF_H

#include <QObject>
#include <QDateTime>

// mail|subscribe|provider|last_date
class SubscribeConf : public QObject
{
  Q_OBJECT
public:
  explicit SubscribeConf(QObject *parent = 0);

  void setPath(QString);
  int  size();
  int  iterator();
  int  iteratorActive();
  int  sizeActive();
  bool next();
  bool nextActive();
  bool update();
  bool addUp(QString); // update с ошибкой
  int  isFolder();
  QString isMail(QString);
  QString isMail();


//  bool addNew();
//  bool removeIs();

  QString name_sender;
  QString theme;
  QString attach;
  QString base_name;
  QString mail_log;
  QString error_log;
  QString message_path;
  int per_day;
  int posted_today;
  QDateTime date;
  int interval;
  bool active;

  QString path_subscribes;

signals:

public slots:

private:

  QString mail;

  int size_all;
  int size_active;
  int iter;
  int iter_active;
  QList<int> all_subscribe;
  int subscribe;
  int folderToNumber(QString);
  bool prepareConfig(QString);
  QString date_format;
  bool isActive();
  bool isToday(QDateTime);
  bool thatIsActive();
  bool readSubscribe(QString);
  QString updateString();
  QString numberToFolder(int);
  QStringList removeBadMail(QStringList);
  bool mailReset();
};

#endif // SUBSCRIBECONF_H
