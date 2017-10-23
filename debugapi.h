#ifndef DEBUGAPI_H
#define DEBUGAPI_H

#include <QByteArray>

class DebugAPI
{
public:
  DebugAPI();
  QByteArray addUser();
  QByteArray addCargo ();
};

#endif // DEBUGAPI_H
