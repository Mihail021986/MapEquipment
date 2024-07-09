#ifndef READER_H
#define READER_H
#include <QThread>

class Reader:public QThread
{Q_OBJECT
public:
   explicit Reader(QObject *parent = 0, QString fn="");
   void run();
private:
    QString fileName;
};

#endif // READER_H
