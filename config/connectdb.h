#ifndef CONNECTDB_H
#define CONNECTDB_H

#include <QObject>

class ConnectDB : public QObject
{
  Q_OBJECT
public:
  explicit ConnectDB(QObject *parent = 0);

  bool createConnectionMYSQLite(QString);
  bool createConnection();

signals:

public slots:
};

#endif // CONNECTDB_H
