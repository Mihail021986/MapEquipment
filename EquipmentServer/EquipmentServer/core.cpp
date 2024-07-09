#include "core.h"
#include <QDir>
#include <iostream>
Block block;
Core::Core(QObject *parent):
    QTcpServer(parent)
{

}
void Core::startServer()
{
    setlocale(LC_ALL, "");
    int port = 1234;

    if(!this->listen(QHostAddress::Any,port))
    {
        qDebug() << "Could not start server";
    }
    else
    {
        qDebug() << "Listening to port " << port << "...";
        QDir MapsDevices("config_equipment");
        if(MapsDevices.exists()){
           MapsDevices.setFilter(QDir::Files);
           QFileInfoList fileList=MapsDevices.entryInfoList();
           block.dateEdition=fileList[0].lastModified().toLocalTime();
           Reader *r=new Reader(this, fileList[0].filePath());
           r->start();
           r->wait();
           for(int i=0; i<fileList.count(); i++){
               Writer *w=new Writer(block,this);
               w->start();
               if(i!=fileList.count()-1){
                   r=new Reader(this, fileList[i+1].filePath());
                   r->start();
                   r->wait();}
               w->wait();
           }
           WaitUpdate *wu=new WaitUpdate(this);
           wu->start();
        }
        else{
            std::wcout<<QString("Не удалось найти директорию с конфигурацией оборудования").toStdWString().c_str()<<std::endl;
        }
    }
}

void Core::incomingConnection(qintptr socketDescriptor)
{
    // We have a new connection
    qDebug() << socketDescriptor << " Connecting...";

    Worker *thread = new Worker(socketDescriptor, this);

    // connect signal/slot
    // once a thread is not needed, it will be beleted later
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

    thread->start();
}
