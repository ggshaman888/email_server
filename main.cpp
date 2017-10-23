<<<<<<< HEAD
#include <QtCore/QCoreApplication>
#include <debugapi.h>
#include <QDebug>
#include <QPluginLoader>
#include <qsimpleserver.h>
#include <QObject>
#include "providersmtp.h"
#include "serveremail.h"
#include "subscribeconfig.h"
#include "dbreload.h"
#include "taskthread.h"
#include "transportmail.h"
#include "reportsubscribe.h"
#include <QDir>
#include "config/dbsqliteconf.h"
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    qDebug() << QDir::home();
/*
    DBSQliteConf *cdb = new DBSQliteConf;
    cdb->createDefault();

 //   DBReload *r = new DBReload;
    //r->replaseDefault();
//    r->addSubscribeMail();
 //   ServerEmail *m = new ServerEmail;
 //  SubscribeConfig subscribe;

//    m->reqestServer("ok", 1);
/*
    ReportSubscribe *report = new ReportSubscribe;

    TaskThred *rep = new TaskThred;
    report->moveToThread(rep);
    rep->start();
    report->reportAllSubscribe();
*/
    TransportMail *tm = new TransportMail;
    TaskThred *trmail = new TaskThred;
    tm->moveToThread(trmail);
    trmail->start();
    tm->startDefault();

    //   m->TESTreqestServer("ok", 1);


 //   QSimpleServer *server = new QSimpleServer;
  //  TaskThred *tr = new TaskThred;

 //   server->moveToThread(tr);
//    tr->start();

//    QObject::connect(&server, SIGNAL(requestServer(QString,int)), m, SLOT(reqestServer(QString,int)));
 //   QObject::connect(m, SIGNAL(returnServer(QString,int)),&server, SIGNAL(returnServer(QString,int)));

    return a.exec();
}
=======
#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    MainWindow w;
    w.resize(500, 600);
    w.show();

    return a.exec();
}
>>>>>>> 5574fd98999175fd77ab5edbcbae233b5cf92fc2
