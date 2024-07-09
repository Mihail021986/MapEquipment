#include "waitupdate.h"

#include <QDebug>
#include <QDir>
#include <QTimeZone>
#include <QFile>
#include "reader.h"
#include "updater.h"
#include "global.h"
#include <iostream>
WaitUpdate::WaitUpdate(QObject *parent):QThread(parent)
{
   this->DateStart=QDateTime::currentDateTime();
}
void WaitUpdate::run(){
    std::wcout<<QString("Запущена задача мониторинга изменений.").toStdWString().c_str()<<std::endl;
    while(true){
        QThread::sleep(60);
        QDir MapsDevices("config_equipment");
        if(MapsDevices.exists()){
            MapsDevices.setFilter(QDir::Files);
            QFileInfoList fileList=MapsDevices.entryInfoList();
            Updater *u;
            for(int i=0; i<fileList.count(); i++){
                QFileInfo *fi=new QFileInfo(fileList[i].filePath());
                if(fi->lastModified().toLocalTime()>this->DateStart){
                    Reader *r=new Reader(this, fi->filePath());
                    if(u!=NULL)
                       u->wait();
                    r->start();
                    r->wait();
                    u=new Updater(block, this);
                    u->start();
                }
            }
        }
    }
}
