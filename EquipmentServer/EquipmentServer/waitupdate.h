#ifndef WAITUPDATE_H
#define WAITUPDATE_H

#include <QDateTime>
#include <QThread>



class WaitUpdate:public QThread
{Q_OBJECT
public:
    WaitUpdate(QObject *parent=0);
    void run();
private:
    QDateTime DateStart;
};

#endif // WAITUPDATE_H
