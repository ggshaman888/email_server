#ifndef LOG_H
#define LOG_H

#include <QObject>

class Log : public QObject
{
  Q_OBJECT
public:
  explicit Log(QObject *parent = 0);

  void setLogFile(QString);

signals:

public slots:
  void addLog(QString);

private:

  QString log_file;
};

#endif // LOG_H
