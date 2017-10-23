  #ifndef TASKCONFIG_H
  #define TASKCONFIG_H

  #include <QObject>
  #include <QStringList>
  #include <QDateTime>

  class TaskConfig : public QObject
  {
    Q_OBJECT
  public:
    explicit TaskConfig(QObject *parent = 0);

    void setConfig();
    void readConfig(QString);
    void readConfigTasks(QString);

    QStringList isErrorLog();
    QStringList isMailLog();
    QStringList isMailBase();
    QStringList isMailErrorLogTrue();
    int isTimeStart(QStringList);
    int isTimeStart(QString);

    void addMailBase(QStringList);

    void clearMailLog();
    void clearErrorLog();
    void clearMailBase();
    void clearAll();

    QString is_folder;
    QString is_file_config;
    QString is_file_base_mail;
    QString is_file_mail_log;
    QString is_file_error_log;
    QString is_file_message;
    QString is_file_attach;
    QString is_name_sender;
    QString is_theme;
    QString is_login;
    QString is_password;
    int     is_per_day;
    int     is_timer;
    bool    is_active;

    QDateTime is_last_message;

    int     is_last_per_day;


  signals:

  public slots:

  private:

    QString path_tasks;
    QString date_format;

    QString readStrError(QStringList, int);
    QString clrMail(QStringList, int);
    QStringList config();

    QString strErrorLogToMail(QString);
    QString clrFolderName(QString);
  };

  #endif // TASKCONFIG_H
