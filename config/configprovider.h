#ifndef CONFIGPROVIDER_H
#define CONFIGPROVIDER_H

#include <QObject>
#include <QDateTime>

class ConfigProvider : public QObject
{
  Q_OBJECT
public:
  explicit ConfigProvider(QObject *parent = 0);

  void setPath(QString);

  bool nextActive();
  bool addUp(QString); // update с ошибкой и конфигурацией
  int  sizeActive();

  bool update();
  int  size();
  int  iterator();
  int  iteratorActive();
  bool next();

  bool addNew();
  bool removeIs();

  bool setIterProvider(QString);

  QString provider;
  QString pass;
  int max_count;
  int min_count_to_error;
  int error_count;
  int per_day_recomendation;
  int posted_today;
  int error_today;
  QDateTime last_datatime;
  bool active;

signals:

public slots:

private:
  bool read();
  int iter;
  int iter_active;
  int size_all;
  int size_active;
  QString path_file;
  QString date_time_format;
  QString date_format;
  QString updatedString();
  QStringList updatedList(QStringList);
  bool activeTrue(QStringList);
  bool isToday(QDateTime);
  bool readActive();
};

#endif // CONFIGPROVIDER_H
