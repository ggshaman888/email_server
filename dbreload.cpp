#include <QFile>
#include <QStringList>
#include <QDebug>
#include <QTextStream>
#include <QMap>
#include <QStringList>

#include "dbreload.h"
#include "taskconfig.h"

DBReload::DBReload(QObject *parent) : QObject(parent)
{

}

void DBReload::setSubscribeTasksName(QStringList task_0)
{
  QStringList task_1;

  task_1 << "/home/alexandr/mutt/tasks/_0015/";
  task_1 << "/home/alexandr/mutt/tasks/0016/";
  task_1 << "/home/alexandr/mutt/tasks/_0017/";
  task_1 << "/home/alexandr/mutt/tasks/_0018/";
  task_1 << "/home/alexandr/mutt/tasks/0033/";
  task_1 << "/home/alexandr/mutt/tasks/0034/";
  task_1 << "/home/alexandr/mutt/tasks/0035/";
  task_1 << "/home/alexandr/mutt/tasks/0036/";
  task_1 << "/home/alexandr/mutt/tasks/0037/";
  task_1 << "/home/alexandr/mutt/tasks/0038/";
  task_1 << "/home/alexandr/mutt/tasks/0039/";
  task_1 << "/home/alexandr/mutt/tasks/0040/";
  task_1 << "/home/alexandr/mutt/tasks/0041/";
  task_1 << "/home/alexandr/mutt/tasks/0042/";
  task_1 << "/home/alexandr/mutt/tasks/0043/";
  task_1 << "/home/alexandr/mutt/tasks/0044/";
  task_1 << "/home/alexandr/mutt/tasks/0045/";
  task_1 << "/home/alexandr/mutt/tasks/0046/";
  task_1 << "/home/alexandr/mutt/tasks/0047/";
  task_1 << "/home/alexandr/mutt/tasks/0049/";
  task_1 << "/home/alexandr/mutt/tasks/0050/";
  task_1 << "/home/alexandr/mutt/tasks/0051/";
  task_1 << "/home/alexandr/mutt/tasks/0052/";
  task_1 << "/home/alexandr/mutt/tasks/0053/";
  task_1 << "/home/alexandr/mutt/tasks/0054/";
  task_1 << "/home/alexandr/mutt/tasks/0055/";

  subscribe_tasks = task_1;

  if (!task_0.isEmpty())
    subscribe_tasks = task_0;
}

void DBReload::setTrueTasks(QStringList task_0)
{
  QStringList task_1;

  task_1 << "/home/alexandr/mutt/tasks/0004/";
  task_1 << "/home/alexandr/mutt/tasks/0011__/";
  task_1 << "/home/alexandr/mutt/tasks/0012__/";
  task_1 << "/home/alexandr/mutt/tasks/0013__/";
  task_1 << "/home/alexandr/mutt/tasks/0014__/";
  task_1 << "/home/alexandr/mutt/tasks/0016/";
  task_1 << "/home/alexandr/mutt/tasks/0023__/";
  task_1 << "/home/alexandr/mutt/tasks/0024__/";
  task_1 << "/home/alexandr/mutt/tasks/0025__/";
  task_1 << "/home/alexandr/mutt/tasks/0026__/";
  task_1 << "/home/alexandr/mutt/tasks/0027__/";
  task_1 << "/home/alexandr/mutt/tasks/0028__/";
  task_1 << "/home/alexandr/mutt/tasks/0029__/";
  task_1 << "/home/alexandr/mutt/tasks/0030__/";
  task_1 << "/home/alexandr/mutt/tasks/0031__/";
  task_1 << "/home/alexandr/mutt/tasks/0032__/";
  task_1 << "/home/alexandr/mutt/tasks/0033/";
  task_1 << "/home/alexandr/mutt/tasks/0034/";
  task_1 << "/home/alexandr/mutt/tasks/0035/";
  task_1 << "/home/alexandr/mutt/tasks/0036/";
  task_1 << "/home/alexandr/mutt/tasks/0037/";
  task_1 << "/home/alexandr/mutt/tasks/0038/";
  task_1 << "/home/alexandr/mutt/tasks/0039/";
  task_1 << "/home/alexandr/mutt/tasks/0040/";
  task_1 << "/home/alexandr/mutt/tasks/0041/";
  task_1 << "/home/alexandr/mutt/tasks/0042/";
  task_1 << "/home/alexandr/mutt/tasks/0043/";
  task_1 << "/home/alexandr/mutt/tasks/0044/";
  task_1 << "/home/alexandr/mutt/tasks/0045/";
  task_1 << "/home/alexandr/mutt/tasks/0046/";
  task_1 << "/home/alexandr/mutt/tasks/0047/";
  task_1 << "/home/alexandr/mutt/tasks/0049/";
  task_1 << "/home/alexandr/mutt/tasks/0050/";
  task_1 << "/home/alexandr/mutt/tasks/0051/";
  task_1 << "/home/alexandr/mutt/tasks/0052/";
  task_1 << "/home/alexandr/mutt/tasks/0053/";
  task_1 << "/home/alexandr/mutt/tasks/0054/";
  task_1 << "/home/alexandr/mutt/tasks/0055/";
  task_1 << "/home/alexandr/mutt/tasks/_0005/";
  task_1 << "/home/alexandr/mutt/tasks/_0006_/";
  task_1 << "/home/alexandr/mutt/tasks/_0007_/";
  task_1 << "/home/alexandr/mutt/tasks/_0008_/";
  task_1 << "/home/alexandr/mutt/tasks/_0009_/";
  task_1 << "/home/alexandr/mutt/tasks/_0010_/";
  task_1 << "/home/alexandr/mutt/tasks/_0015/";
  task_1 << "/home/alexandr/mutt/tasks/_0017/";
  task_1 << "/home/alexandr/mutt/tasks/_0018/";
  task_1 << "/home/alexandr/mutt/tasks/_0019/";
  task_1 << "/home/alexandr/mutt/tasks/_0020_/";
  task_1 << "/home/alexandr/mutt/tasks/_0021_/";
  task_1 << "/home/alexandr/mutt/tasks/_0022_/";

  true_tasks = task_1;

  if (!task_0.isEmpty())
    subscribe_tasks = task_0;
}

void DBReload::setPathTasks(QString folder)
{
  path_folder = folder;
}

void DBReload::replaseDefault()
{
  taskSetPath();
  for (int i=0; i<task.size();i++)
    {
      setSubscribeTasksName(task.at(i));
      ++i;
      setTrueTasks(task.at(i));
      goReplase();
    }
}

void DBReload::replase()
{
  QStringList list;
  setSubscribeTasksName(list);
  setTrueTasks(list);

  goReplase();
}



void DBReload::fill_mail_base()
{
  for(int i=0; i<true_tasks.size();i++)
    task_to_mail_base(i);
  delErrorBase();
}

void DBReload::task_to_mail_base(int x)
{
  TaskConfig *task = new TaskConfig;
  task->readConfig(pathFolderTask(x));

  QStringList base;
  base.append(task->isMailBase());
  base.append(task->isMailLog());

  base.removeDuplicates();

  for(int i=0;i<base.size();i++)
    toMailBaseIfTrue(base.at(i));

}

QString DBReload::pathFolderTask(int x)
{
  return true_tasks.at(x);
}

void DBReload::toMailBaseIfTrue(QString mail)
{
  QStringList buf, buf2;
  if (mail.size()>3)
    {
      mail.replace(10, "");
      mail.replace(" ", "");
      if (base_mail.indexOf(mail)==-1)
        if (mail.indexOf(","))
          {
            buf = mail.split(",");
            for (int i=0;i<buf.size();i++)
              {
                if (mail.indexOf(";")>-1)
                  {
                    buf2 = mail.split(";");
                    for(int j=0; j<buf2.size();j++)
                      base_mail << buf2.at(j);
                  }
                base_mail << buf.at(i);
              }
          }
        else
          {
            if (mail.indexOf(";")>-1)
              {
                buf2 = mail.split(";");
                for(int j=0; j<buf2.size();j++)
                  base_mail << buf2.at(j);
              }
            base_mail << mail;
          }
    }
}

void DBReload::clear_task(int x)
{
  QString str = subscribe_tasks.at(x);

  QFile file;
  file.setFileName(str.append("base.csv"));
  file.remove();
  file.open(QIODevice::WriteOnly);
  file.close();

  str = pathFolderTask(x);

  file.setFileName(str.append("email.log"));
  file.remove();
  file.open(QIODevice::WriteOnly);
  file.close();
}

void DBReload::mailToTask(int x)
{
  qDebug() << x;
  QString str = subscribe_tasks.at(x);
  TaskConfig *task = new TaskConfig;
  task->readConfig(str);

  int count = task->is_per_day;

  QString base_csv = baseSubscribe(x);

  QFile file;
  file.setFileName(base_csv); //str.append("base.csv")
  file.open(QIODevice::Append | QIODevice::Text);
  QTextStream out(&file);

  for(int i=0; i<count; i++)
    {
      if (!base_mail.isEmpty())
        out << readBaseMail(i);
      else break;
    }

  file.close();

}

QString DBReload::readBaseMail(int x)
{
  qDebug() << x;
  QString mail = mailMaxCount();
  removeMailIsBase(mail);

  return mail.append(10);
}

void DBReload::mailCount()
{
  QList<int> list;
  for(int j=0; j<base_mail.size();j++)
    {
      list.clear();
      for(int i=0; i<subscribe_tasks.size(); i++)
        list  << countMailTask(base_mail.at(j), i);

      qSort(list);
      mail_count.insert(base_mail.at(j), list.last());
    }
}

int DBReload::countMailTask(QString mail, int id)
{
  TaskConfig *task = new TaskConfig;
  task->readConfig(pathFolderTask(id));

  QStringList base = task->isMailBase();
  base.append(task->isMailErrorLogTrue());

  int count = 0;
  qCount(base.begin(), base.end(), mail, count);


  return count;
}

QString DBReload::mailMaxCount()
{
  QString mail;
  int max=0;
  QMap<QString, int>::const_iterator i = mail_count.constBegin();
  while (i != mail_count.constEnd()) {
      if (i.value()>max)
        max = i.value();
      ++i;
  }
  i = mail_count.constBegin();
  while (i != mail_count.constEnd()) {
      if (i.value()==max)
        {
          mail = i.key();
          break;
        }
      ++i;
  }

  return mail;
}

void DBReload::removeMailIsBase(QString mail)
{
  base_mail.removeAll(mail);
  mail_count.remove(mail);
}

void DBReload::taskSetPath()
{
  QStringList task_1;
  task_1 << "/home/alexandr/mutt/tasks/0004/";
  task_1 << "/home/alexandr/mutt/tasks/0011__/";
  task_1 << "/home/alexandr/mutt/tasks/0012__/";
  task_1 << "/home/alexandr/mutt/tasks/0013__/";
  task_1 << "/home/alexandr/mutt/tasks/0014__/";
  task_1 << "/home/alexandr/mutt/tasks/0016/";
  task_1 << "/home/alexandr/mutt/tasks/0023__/";
  task_1 << "/home/alexandr/mutt/tasks/0024__/";
  task_1 << "/home/alexandr/mutt/tasks/0025__/";
  task_1 << "/home/alexandr/mutt/tasks/0026__/";
  task_1 << "/home/alexandr/mutt/tasks/0027__/";
  task_1 << "/home/alexandr/mutt/tasks/0028__/";
  task_1 << "/home/alexandr/mutt/tasks/0029__/";
  task_1 << "/home/alexandr/mutt/tasks/0030__/";
  task_1 << "/home/alexandr/mutt/tasks/0031__/";
  task_1 << "/home/alexandr/mutt/tasks/0032__/";
  task_1 << "/home/alexandr/mutt/tasks/0033/";
  task_1 << "/home/alexandr/mutt/tasks/0034/";
  task_1 << "/home/alexandr/mutt/tasks/0035/";
  task_1 << "/home/alexandr/mutt/tasks/0036/";
  task_1 << "/home/alexandr/mutt/tasks/0037/";
  task_1 << "/home/alexandr/mutt/tasks/0038/";
  task_1 << "/home/alexandr/mutt/tasks/0039/";
  task_1 << "/home/alexandr/mutt/tasks/0040/";
  task_1 << "/home/alexandr/mutt/tasks/0041/";
  task_1 << "/home/alexandr/mutt/tasks/0042/";
  task_1 << "/home/alexandr/mutt/tasks/0043/";
  task_1 << "/home/alexandr/mutt/tasks/0044/";
  task_1 << "/home/alexandr/mutt/tasks/0045/";
  task_1 << "/home/alexandr/mutt/tasks/0046/";
  task_1 << "/home/alexandr/mutt/tasks/0047/";
  task_1 << "/home/alexandr/mutt/tasks/0049/";
  task_1 << "/home/alexandr/mutt/tasks/0050/";
  task_1 << "/home/alexandr/mutt/tasks/0051/";
  task_1 << "/home/alexandr/mutt/tasks/0052/";
  task_1 << "/home/alexandr/mutt/tasks/0053/";
  task_1 << "/home/alexandr/mutt/tasks/0054/";
  task_1 << "/home/alexandr/mutt/tasks/0055/";
  task_1 << "/home/alexandr/mutt/tasks/_0005/";
  task_1 << "/home/alexandr/mutt/tasks/_0006_/";
  task_1 << "/home/alexandr/mutt/tasks/_0007_/";
  task_1 << "/home/alexandr/mutt/tasks/_0008_/";
  task_1 << "/home/alexandr/mutt/tasks/_0009_/";
  task_1 << "/home/alexandr/mutt/tasks/_0010_/";
  task_1 << "/home/alexandr/mutt/tasks/_0015/";
  task_1 << "/home/alexandr/mutt/tasks/_0017/";
  task_1 << "/home/alexandr/mutt/tasks/_0018/";
  task_1 << "/home/alexandr/mutt/tasks/_0019/";
  task_1 << "/home/alexandr/mutt/tasks/_0020_/";
  task_1 << "/home/alexandr/mutt/tasks/_0021_/";
  task_1 << "/home/alexandr/mutt/tasks/_0022_/";

  QStringList task_2; // он же 1-3
  task_2 << "/home/alexandr/mutt/tasks/0001/";
  task_2 << "/home/alexandr/mutt/tasks/0002/";
  task_2 << "/home/alexandr/mutt/tasks/_0003/";

  QStringList task_4_10;
  task_4_10 << "/home/alexandr/mutt/tasks/0004/";
  task_4_10 << "/home/alexandr/mutt/tasks/_0005/";
  task_4_10 << "/home/alexandr/mutt/tasks/_0006_/";
  task_4_10 << "/home/alexandr/mutt/tasks/_0007_/";
  task_4_10 << "/home/alexandr/mutt/tasks/_0008_/";
  task_4_10 << "/home/alexandr/mutt/tasks/_0009_/";
  task_4_10 << "/home/alexandr/mutt/tasks/_0010_/";

  QStringList task_11_14;
  task_11_14 << "/home/alexandr/mutt/tasks/0011__/";
  task_11_14 << "/home/alexandr/mutt/tasks/0012__/";
  task_11_14 << "/home/alexandr/mutt/tasks/0013__/";
  task_11_14 << "/home/alexandr/mutt/tasks/0014__/";

  QStringList task_15_18;

  task_15_18 << "/home/alexandr/mutt/tasks/_0015/";
  task_15_18 << "/home/alexandr/mutt/tasks/0016/";
  task_15_18 << "/home/alexandr/mutt/tasks/_0017/";
  task_15_18 << "/home/alexandr/mutt/tasks/_0018/";
  task_15_18 << "/home/alexandr/mutt/tasks/0033/";
  task_15_18 << "/home/alexandr/mutt/tasks/0034/";
  task_15_18 << "/home/alexandr/mutt/tasks/0035/";
  task_15_18 << "/home/alexandr/mutt/tasks/0036/";
  task_15_18 << "/home/alexandr/mutt/tasks/0037/";
  task_15_18 << "/home/alexandr/mutt/tasks/0038/";
  task_15_18 << "/home/alexandr/mutt/tasks/0039/";
  task_15_18 << "/home/alexandr/mutt/tasks/0040/";
  task_15_18 << "/home/alexandr/mutt/tasks/0041/";
  task_15_18 << "/home/alexandr/mutt/tasks/0042/";
  task_15_18 << "/home/alexandr/mutt/tasks/0043/";
  task_15_18 << "/home/alexandr/mutt/tasks/0044/";
  task_15_18 << "/home/alexandr/mutt/tasks/0045/";
  task_15_18 << "/home/alexandr/mutt/tasks/0046/";
  task_15_18 << "/home/alexandr/mutt/tasks/0047/";
  task_15_18 << "/home/alexandr/mutt/tasks/0049/";
  task_15_18 << "/home/alexandr/mutt/tasks/0050/";
  task_15_18 << "/home/alexandr/mutt/tasks/0051/";
  task_15_18 << "/home/alexandr/mutt/tasks/0052/";
  task_15_18 << "/home/alexandr/mutt/tasks/0053/";
  task_15_18 << "/home/alexandr/mutt/tasks/0054/";
  task_15_18 << "/home/alexandr/mutt/tasks/0055/";

  QStringList task_56;

  task_56 << "/home/alexandr/mutt/tasks/0056/";
  task_56 << "/home/alexandr/mutt/tasks/0057/";
  task_56 << "/home/alexandr/mutt/tasks/0058/";

  task << task_2 << task_2
       << task_4_10 << task_1
       << task_11_14 << task_1
       << task_15_18 << task_1
       << task_56 << task_56;
}

void DBReload::goReplase()
{

  fill_mail_base();
  mailCount();

  for(int i=0;i<subscribe_tasks.size();i++)
    clear_task(i);

  while(!base_mail.isEmpty())
    for(int i=0;i<subscribe_tasks.size();i++)
      if (!base_mail.isEmpty())
        mailToTask(i);
}

void DBReload::delErrorBase()
{
  QStringList list;
  QFile file;
  file.setFileName("/home/alexandr/mutt/error_mail.txt");
  if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    while (!file.atEnd())
      list << file.readLine();
  file.close();

  for(int i=0; i<list.size();i++)
    base_mail.removeAll(list.at(i));
}

QString DBReload::baseSubscribe(int id)
{

    QString name = subscribe_tasks.at(id);

    QRegExp reg("\\d");
    name = name.mid(reg.indexIn(name), 4);

    if (name[0]=='0') name.remove(0, 1);
    if (name[0]=='0') name.remove(0, 1);
    if (name[0]=='0') name.remove(0, 1);

    int x = name.toInt();

    if (x>0 and x<4) return   "/home/alexandr/mutt/subscribe/0001/base.csv";
    if (x>3 and x<11) return  "/home/alexandr/mutt/subscribe/0002/base.csv";
    if (x>18 and x<23) return "/home/alexandr/mutt/subscribe/0002/base.csv";
    if (x>45 and x<56) return "/home/alexandr/mutt/subscribe/0002/base.csv";
    if (x>10 and x<15) return "/home/alexandr/mutt/subscribe/0003/base.csv";
    if (x>22 and x<33) return "/home/alexandr/mutt/subscribe/0003/base.csv";
    if (x>14 and x<19) return "/home/alexandr/mutt/subscribe/0004/base.csv";
    if (x>32 and x<46) return "/home/alexandr/mutt/subscribe/0004/base.csv";
    if (x>55 and x<59) return "/home/alexandr/mutt/subscribe/0005/base.csv";
return "";
}

void DBReload::addSubscribeMail()
{
  QString input_file = "/home/alexandr/mutt/subscribe/base_moskov.csv";
  QString output_file = "/home/alexandr/mutt/subscribe/0001/base.csv";
  QString error_mail_file = "/home/alexandr/mutt/error_mail.txt";

  base_mail.clear();

  QFile file;
  file.setFileName(input_file);
  if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    while (!file.atEnd())
      toMailBaseIfTrue(file.readLine());
  file.close();

  qDebug() << base_mail.size();
  base_mail.removeDuplicates();
  qDebug() << base_mail.size();
  QString buf;
  file.setFileName(error_mail_file);
  if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    while (!file.atEnd())
      {
        buf = file.readLine();
        buf.resize(buf.size()-1);
        base_mail.removeOne(buf);
      }
  file.close();


  file.setFileName(output_file);
  if(file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
      QTextStream out(&file);
      for(int i=0; i<base_mail.size();i++)
        out << base_mail.at(i) << "\n";
    }
  file.close();

  qDebug() << base_mail.size();
}
