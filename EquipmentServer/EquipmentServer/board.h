#ifndef BOARD_H
#define BOARD_H

#include <QString>
#include <QList>
#include "port.h"

class Board
{
public:
    Board();
    long ID;
    QString name;
    long num;
    long portCount;
    QString intLinks;
    QString Algoritms;
    QList<Port> ports;
};

#endif // BOARD_H
