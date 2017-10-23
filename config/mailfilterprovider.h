#ifndef MAILFILTERPROVIDER_H
#define MAILFILTERPROVIDER_H

#include <QObject>

class MailFilterProvider : public QObject
{
  Q_OBJECT
public:
  explicit MailFilterProvider(QObject *parent = 0);

  bool mailFilterProvider(QString, QString);

signals:

public slots:
};

#endif // MAILFILTERPROVIDER_H
