#ifndef WRITER_H
#define WRITER_H

#include "block.h"

#include <QThread>
#include <QDomDocument>
#include <QtSql>
class Writer:public QThread
{Q_OBJECT
public:
    Writer(Block b, QObject *parent = 0);
    void run();
private:
    Block block;
};

#endif // WRITER_H
