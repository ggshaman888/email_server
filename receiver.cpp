#include "receiver.h"
#include <QDebug>
#include "serveremail.h"
#include <QStringList>

Receiver::Receiver(QObject *parent) : QObject(parent)
{

}

void Receiver::toServer(QByteArray message, int x)
{
  qDebug() << message << x;

  switch (control(message)) {
    case 1:
      {
       // emit outServer("message", x);
        ServerEmail *m = new ServerEmail;
        m->reqestServer("ok", 1);

       break;
      }
  /*  case 2:
      testMessage(); break;
    case 3:
      statistics();  break;
    case 4:
      stopResponder(); break;
    case 5:
      status(); break;
*/
    default:
      emit outServer(message, x);
      break;
    }
}

int Receiver::control(QByteArray message)
{
  QString str = message;
  str.mid(str.indexOf("!"), str.lastIndexOf("!"));

  int x;

  if (str.indexOf("exit")) x=1;
  if (str=="test_message") x=2;
  if (str=="statistics") x=3;
  if (str=="stop_responder") x=4;
  if (str=="status") x=5;

  return x;
}

