#ifndef UPDATER_H
#define UPDATER_H

#include "block.h"

#include <QThread>



class Updater:public QThread
{Q_OBJECT
public:
    Updater(Block b, QObject *parent);
    void run();
private:
    Block block;
};

#endif // UPDATER_H
