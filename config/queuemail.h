#ifndef QUEUEMAIL_H
#define QUEUEMAIL_H

#include <QObject>
#include <config/subscribeconf.h>
#include <config/configprovider.h>
#include <config/muttconfig.h>
#include <QMultiMap>
#include <QStringList>

class QueueMail : public QObject
{
  Q_OBJECT
public:
  explicit QueueMail(QObject *parent = 0);

  void setSubscribePath(QString);

  void push(QString, QString);

  MuttConfig pop();
  void removeTaskProvider(QString);

  void distribute(QString);

signals:

public slots:

private:
  QStringList subscribe;
  QString this_subscribe;
  QList<int> added_count;
  QList<int> max_count;
  QList<int> min_interval;
  QMap <int, QList<QStringList> > queue; // minutes / provider|subscribe
  int duration_message; // ms
  int range; // m 1440

  QString path_subscribe;
  int countLeft();
  int countProvider(QString);
  void addQueue(QString, int);
};

#endif // QUEUEMAIL_H
