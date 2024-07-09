#ifndef WORKER_H
#define WORKER_H
#include <QThread>
#include <QTcpSocket>
class Worker : public QThread
{
    Q_OBJECT
public:
   explicit Worker(qintptr ID, QObject *parent = 0);
    void run();
signals:
   void error(QTcpSocket::SocketError socketerror);
public slots:
   void readyRead();
   void disconnected();
private:
    QTcpSocket *socket;
    qintptr socketDescriptor;
};

#endif // WORKER_H
