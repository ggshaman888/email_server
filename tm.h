#ifndef TM_H
#define TM_H

#include <QObject>

class TM : public QObject
{
  Q_OBJECT
public:
  explicit TM(QObject *parent = 0);

  void tmDefault();

signals:

public slots:
};

#endif // TM_H
