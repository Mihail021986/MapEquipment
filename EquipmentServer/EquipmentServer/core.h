#ifndef CORE_H
#define CORE_H
#include <QTcpServer>
#include "worker.h"
#include "block.h"
#include "reader.h"
#include "writer.h"
#include "worker.h"
#include "global.h"
#include "waitupdate.h"
class Core: public QTcpServer
{Q_OBJECT
public:
    Core(QObject *parent = 0);
    void startServer();
protected:
   void incomingConnection(qintptr socketDescriptor);
};

#endif // CORE_H
