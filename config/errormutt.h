#ifndef ERRORMUTT_H
#define ERRORMUTT_H

#include <QObject>
#include <QStringList>

class ErrorMutt : public QObject
{
  Q_OBJECT
public:
  explicit ErrorMutt(QObject *parent = 0);

  QString errorStatus(QString);
  QStringList infoErrorStatus();

signals:

public slots:

private:
  QStringList error_list;
  QStringList error_critical;
  QStringList error_bad_mail;
  QStringList error_accsess;

  void toLogNoKnownError();
  QString error;
  QString path_error_no_known;
  QStringList error_status;
};

#endif // ERRORMUTT_H
