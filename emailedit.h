#ifndef EMAILEDIT_H
#define EMAILEDIT_H

#include <QObject>
#include <QString>

class EmailEdit : public QObject
{
  Q_OBJECT
public:
  explicit EmailEdit(QObject *parent = 0);

  bool emailTrue (QString);
  QStringList emailExtract(QString); // проверить выдачу

signals:

public slots:

private:
  int right(QChar);
  int left(QChar);
  bool domain(QString);
};

#endif // EMAILEDIT_H
