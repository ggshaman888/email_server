#include "config/dbsqliteconf.h"
#include "config/dbsqliteconf.h"
#include "config/connectdb.h"
#include <QFile>
#include <QTextStream>
#include <QTextCodec>
#include <QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlDriver>
#include <QtSql/QtSql>

DBSQliteConf::DBSQliteConf(QObject *parent) : QObject(parent)
{

}

void DBSQliteConf::setPath(QString file)
{
  path_db = file;
}

bool DBSQliteConf::createDefault()
{
  QFile file(path_db);
//  backupDB(path_db);

  if(file.open(QIODevice::WriteOnly))
    file.close();
  // проверить создание

  return createTablesDefault();
}

bool DBSQliteConf::createTablesDefault()
{
  ConnectDB *c = new ConnectDB;
  if(c->createConnectionMYSQLite(path_db))
    {
      QSqlQuery query_sqlite(QSqlDatabase::database("dbsqlite"));

      query_sqlite.prepare(
        "CREATE TABLE `log` ("
        "`id`           INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,"
        "`provider`     TEXT,"
        "`email`        TEXT,"
        "`data_time`    TEXT,"
        "`subscribe`    INTEGER,"
        ");"
            );

      qDebug()<< "Create defaultDB_table_log" << query_sqlite.exec()
              << query_sqlite.lastError()
              << QDateTime::currentDateTime();

      return true;
    }
  else
    return false;
}
