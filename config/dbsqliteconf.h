#ifndef DBSQLITECONF_H
#define DBSQLITECONF_H

#include <QObject>

class DBSQliteConf : public QObject
{
  Q_OBJECT
public:
  explicit DBSQliteConf(QObject *parent = 0);

  void setPath(QString);
  bool createDefault();

signals:

public slots:

private:

  QString path_db;
  bool createTablesDefault();
};

#endif // DBSQLITECONF_H
