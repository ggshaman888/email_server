#ifndef TRANSPORTMAIL_H
#define TRANSPORTMAIL_H

#include <QObject>
#include "subscribeconfig.h"
#include "config/muttconfig.h"
#include <QStringList>

class TransportMail : public QObject
{
  Q_OBJECT
public:
  explicit TransportMail(QObject *parent = 0);

  void startDefault();

signals:

public slots:

private:
  QString transport(QStringList, QString);
  bool isTimeOut();

};

#endif // TRANSPORTMAIL_H
