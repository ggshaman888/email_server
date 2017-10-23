#include "config/errormutt.h"
#include <QFile>
#include <QTextStream>
#include <QTextCodec>
#include <QStringList>

ErrorMutt::ErrorMutt(QObject *parent) : QObject(parent)
{
  error_critical
      << "SASL authentication failed"
      << "SPAM"
      << "Daily user sending quota exceeded"
      << "SMTP session failed";

  error_bad_mail
      << "SMTP session failed: 553 5.1.2 RFC-5321 address";

  error_accsess
      << "No recipients were specified"
      << "Could not find the host";

  path_error_no_known = "/home/alexandr/mutt/error_mutt_no_known.txt";

  error_status
      << "critical"
      << "mail_bad"
      << "no_known"
      << "accsess"
      << "other";
}

QString ErrorMutt::errorStatus(QString error_str)
{
  error = error_str;

  if(error_list.indexOf(error)!=-1)
    return "other";
  if(error_bad_mail.indexOf(error)!=-1)
    return "bad_mail";
  if(error_critical.indexOf(error)!=-1)
    return "critical";
  if(error_accsess.indexOf(error)!=-1)
    return "accsess";

  toLogNoKnownError();

  return "no_known";
}

QStringList ErrorMutt::infoErrorStatus()
{
  return error_status;
}

void ErrorMutt::toLogNoKnownError()
{
  QFile f(path_error_no_known);
  if(f.open(QIODevice::Append | QIODevice::Text))
    {
      QTextStream out(&f); out.setCodec("UTF-8");
      out << error;
    }
  f.close();
}
