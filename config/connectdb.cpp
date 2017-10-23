#include "config/connectdb.h"
#include <QFile>
#include <QTextStream>
#include <QTextCodec>
#include <QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlDriver>
#include <QtSql/QtSql>

ConnectDB::ConnectDB(QObject *parent) : QObject(parent)
{

}

bool ConnectDB::createConnectionMYSQLite(QString path)
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

bool ConnectDB::createConnection()
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
