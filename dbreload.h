#ifndef DBRELOAD_H
#define DBRELOAD_H

#include <QObject>
#include <QMap>
#include <QStringList>



class DBReload : public QObject
{
  Q_OBJECT
public:
  explicit DBReload(QObject *parent = 0);

  void setSubscribeTasksName(QStringList);
  void setTrueTasks(QStringList);
  void setPathTasks(QString);

  void replaseDefault();
  void replase();

  void addToSQLite();

  void addSubscribeMail();
signals:

public slots:

private:
  QStringList subscribe_tasks;
  QStringList true_tasks;
  QString path_folder;
  QStringList base_mail;
  QMap <QString, int> mail_count;

  void fill_mail_base();
  void task_to_mail_base(int);
  QString pathFolderTask(int);
  void toMailBaseIfTrue(QString);

  void clear_task(int);
  void mailToTask(int);

  QString readBaseMail(int);
  void mailCount();
  int countMailTask(QString, int);

  QString mailMaxCount();
  void removeMailIsBase(QString);

  QList<QStringList> task;
  void taskSetPath();
  void goReplase();
  void delErrorBase();

  QString baseSubscribe(int);
};

#endif // DBRELOAD_H
