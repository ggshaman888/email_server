#ifndef MAILOUT_H
#define MAILOUT_H

#include <QObject>

class MailOut : public QObject
{
  Q_OBJECT
public:
  explicit MailOut(QObject *parent = 0);

  //void setFolderSubscribe (QString); // вносим расположение папки подписки


signals:

public slots:
};

#endif // MAILOUT_H
