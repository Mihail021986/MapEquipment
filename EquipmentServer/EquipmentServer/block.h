#ifndef BLOCK_H
#define BLOCK_H

#include <QDateTime>
#include <QList>
#include <QString>
#include "board.h"


class Block
{
public:
    Block();
    long ID;
    QString name;
    QString IP;
    long boardCount;
    long MtR;
    long MtC;
    QString description;
    QString label;
    QList<Board> boards;
    QDateTime dateEdition;
};

#endif // BLOCK_H
