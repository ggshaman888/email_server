#ifndef REPORTSUBSCRIBE_H
#define REPORTSUBSCRIBE_H
#include <QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlDriver>
#include <QtSql/QtSql>
#include <QObject>
static bool createConnectionMYSQLite(QString path)
{
    QSqlDatabase dbsqlite = QSqlDatabase::addDatabase("QSQLITE", "dbsqlite");
    dbsqlite.setDatabaseName(path);
    qDebug() << dbsqlite.drivers();
    if (!dbsqlite.open()) {
        qDebug() << "Cannot open database: " << dbsqlite.lastError();
        return false;
    }
    return true;
}
static bool createConnection()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL", "dbsql");
    db.setDatabaseName("dgis");
    db.setUserName("root");
    db.setHostName("localhost");
    db.setPassword("golos123");
    qDebug() << db.drivers();
    if (!db.open()) {
        qDebug() << "Cannot open database: " << db.lastError();
        return false;
    }
    return true;
}
class ReportSubscribe : public QObject
{
  Q_OBJECT
public:
  explicit ReportSubscribe(QObject *parent = 0);

  class LogLog
  {
    public:
      QString provider, email, datetime, error, subscribe;
  };
  void reportAllSubscribe();
  void writeAllReportSubscribe();
  void toLogSQLite();
  
signals:

public slots:
  
private:
  void onceNewStrToFile(QString, QString, QString);
  void readLogSubscribe();
  void strParsing(QString);
  void readSubscribeLog(QString);
  void sortFile();
  void addToListFile(int);
  QString strToLog(QString, int);
  QStringList list_file;
  QList <LogLog> log;
  int iter_log;
  QString path_s;
  QString path_sqlite;
  QString thisIsSubscribe(QString);
  bool defineSubscribe(QString, QString);

  void createTmpfs();

};

#endif // REPORTSUBSCRIBE_H
