#include "writer.h"
#include <iostream>
#include <QtSql>
Writer::Writer(Block b, QObject *parent):QThread(parent)
{
    this->block=b;
}
void Writer::run(){
    setlocale(LC_ALL, "");
    if(block.name!="error"){
    QString idThread=QString::number(QDateTime::currentMSecsSinceEpoch());
    QSqlDatabase db=QSqlDatabase::addDatabase("QSQLITE", idThread);
    db.setDatabaseName("../../Equipment.db");
    if(db.open()){
        std::wcout<<QString("Подключение к базе Equipment выполнено успешно. В потоке ").toStdWString().c_str()<<idThread.toLongLong()<<std::endl;
        QSqlQuery selExistBlock(db);
        QList<long> existID;
        if(selExistBlock.exec("SELECT Id FROM Block;"))
        {while (selExistBlock.next()) {
             existID.push_back(selExistBlock.value(0).toString().toLong());
             }
        }
        else {
            std::wcout<<QString("Ошибка чтения данных: ").toStdWString().c_str()<<selExistBlock.lastError().text().toStdWString().c_str()<<std::endl;
            return;
        }
        if(existID.indexOf(this->block.ID)==-1)
        {
        QSqlQuery insBlock(db);
        QString query="INSERT INTO Block VALUES(%1,'%2','%3',%4,%5,%6,'%7','%8', '%9');";
        query=query.arg(this->block.ID)
             .arg(this->block.name)
             .arg(this->block.IP)
             .arg(this->block.boardCount)
             .arg(this->block.MtR)
             .arg(this->block.MtC)
             .arg(this->block.description)
             .arg(this->block.label)
             .arg(QDateTime::currentDateTime().toString());
        if(!insBlock.exec(query)){
            std::wcout<<QString("Ошибка загрузки данных: ").toStdWString().c_str()<<insBlock.lastError().text().toStdWString().c_str()<<std::endl;
            //exec();
        }
        else
            std::wcout<<QString("В таблицу Equipment.Block добавлено ").toStdWString().c_str()<<insBlock.numRowsAffected()<<QString(" записей.").toStdWString().c_str()<<std::endl;
        for(int j=0; j<this->block.boards.count(); j++){
            QSqlQuery insBoard(db);
            query="INSERT INTO Board VALUES(%1,'%2',%3,%4,'%5','%6',%7);";
            query=query.arg(this->block.boards[j].ID)
                 .arg(this->block.boards[j].name)
                 .arg(this->block.boards[j].num)
                 .arg(this->block.boards[j].portCount)
                 .arg(this->block.boards[j].Algoritms)
                 .arg(this->block.boards[j].intLinks)
                 .arg(block.ID);
            if(!insBoard.exec(query)){
                std::wcout<<QString("Ошибка загрузки данных: ").toStdWString().c_str()<<insBoard.lastError().text().toStdWString().c_str()<<std::endl;
               // exec();
            }
            else
                std::wcout<<QString("В таблицу Equipment.Board добавлено ").toStdWString().c_str()<<insBoard.numRowsAffected()<<QString(" записей.").toStdWString().c_str()<<std::endl;
            for(int k=0; k<this->block.boards[j].ports.count(); k++){
                QSqlQuery insPort(db);
                query="INSERT INTO Port VALUES(%1,%2,%3,%4,%5);";
                query=query.arg(this->block.boards[j].ports[k].ID)
                     .arg(this->block.boards[j].ports[k].num)
                     .arg(this->block.boards[j].ID)
                     .arg(this->block.boards[j].ports[k].media)
                     .arg(this->block.boards[j].ports[k].signal);
                if(!insPort.exec(query)){
                    std::wcout<<QString("Ошибка загрузки данных: ").toStdWString().c_str()<<insPort.lastError().text().toStdWString().c_str()<<std::endl;
                    //exec();
                }
                else
                    std::wcout<<QString("В таблицу Equipment.Port добавлено ").toStdWString().c_str()<<insPort.numRowsAffected()<<QString(" записей.").toStdWString().c_str()<<std::endl;
            }
        }
        }
        else{
            QSqlQuery dateModifiedBlock(db);
            QString query="SELECT DateModified FROM Block WHERE Id=%1";
            query=query.arg(block.ID);
            QDateTime dateModified;
            if(dateModifiedBlock.exec(query)){
                while(dateModifiedBlock.next()){
                    dateModified=dateModifiedBlock.value(0).toDateTime();
                }
            }
            else {
                std::wcout<<QString("Ошибка чтения данных: ").toStdWString().c_str()<<selExistBlock.lastError().text().toStdWString().c_str()<<std::endl;
                return;
            }
            if(block.dateEdition>dateModified){
                QSqlQuery upBlock(db);
                QString query;
                query="UPDATE Block SET Name='%1', IP='%2', BoardCount=%3, MtR=%4, MtC=%5, Description='%6', Label='%7', DataModified='%8' WHERE Id=%9;";
                query=query.arg(this->block.name)
                     .arg(this->block.IP)
                     .arg(this->block.boardCount)
                     .arg(this->block.MtR)
                     .arg(this->block.MtC)
                     .arg(this->block.description)
                     .arg(this->block.label)
                     .arg(QDateTime::currentDateTime().toString())
                     .arg(this->block.ID);
                if(!upBlock.exec(query)){
                    std::wcout<<QString("Ошибка обновления данных: ").toStdWString().c_str()<<upBlock.lastError().text().toStdWString().c_str()<<std::endl;
                }
                else
                    std::wcout<<QString("Данные по Block ").toStdWString().c_str()<<this->block.name.toStdWString().c_str()<<" c IP: "<<this->block.IP.toStdWString().c_str()<<QString(" обновлены.").toStdWString().c_str()<<std::endl;
                QSqlQuery selExistBoard(db);
                QList<long> existBoards;
                if(selExistBoard.exec("SELECT Id FROM Board;"))
                {while (selExistBoard.next()) {
                     existBoards.push_back(selExistBoard.value(0).toString().toLong());
                     }
                }
                else {
                    std::wcout<<QString("Ошибка чтения данных: ").toStdWString().c_str()<<selExistBoard.lastError().text().toStdWString().c_str()<<std::endl;
                    return;
                }
                QList<long> existPorts;
                QSqlQuery selExistPort(db);
                if(selExistPort.exec("SELECT Id FROM Port;")){
                    while (selExistPort.next()) {
                        existPorts.push_back(selExistPort.value(0).toString().toLong());
                        }
                }
               else {
                   std::wcout<<QString("Ошибка чтения данных: ").toStdWString().c_str()<<selExistPort.lastError().text().toStdWString().c_str()<<std::endl;
                   return;
               }
                for(int i=0; i<this->block.boards.count(); i++){
                     QSqlQuery upBoard(db);
                     if(existBoards.indexOf(this->block.boards[i].ID)==-1){
                         query="INSERT INTO Board VALUES(%1,'%2',%3,%4,'%5','%6',%7);";
                         query=query.arg(this->block.boards[i].ID)
                              .arg(this->block.boards[i].name)
                              .arg(this->block.boards[i].num)
                              .arg(this->block.boards[i].portCount)
                              .arg(this->block.boards[i].Algoritms)
                              .arg(this->block.boards[i].intLinks)
                              .arg(block.ID);
                     }
                     else{
                         query="UPDATE Board SET Name='%1', Num=%2, PortCount=%3, Algoritms='%4', IntLinks='%5', Id_block=%6 WHERE Id=%7;";
                         query=query.arg(this->block.boards[i].name)
                              .arg(this->block.boards[i].num)
                              .arg(this->block.boards[i].portCount)
                              .arg(this->block.boards[i].Algoritms)
                              .arg(this->block.boards[i].intLinks)
                              .arg(block.ID)
                              .arg(this->block.boards[i].ID);}
                if(!upBlock.exec(query)){
                    std::wcout<<QString("Ошибка обновления данных: ").toStdWString().c_str()<<upBlock.lastError().text().toStdWString().c_str()<<std::endl;
                }
                else
                    std::wcout<<QString("Данные по Board ").toStdWString().c_str()<<this->block.boards[i].name.toStdWString().c_str()<<" c N: "<<this->block.boards[i].num<<QString(" обновлены.").toStdWString().c_str()<<std::endl;
                for(int j=0; j<this->block.boards[i].ports.count(); j++){
                    QSqlQuery upPort(db);
                    if(existPorts.indexOf(this->block.boards[i].ports[j].ID)==-1){
                        query="INSERT INTO Port VALUES(%1,%2,%3,%4,%5);";
                        query=query.arg(this->block.boards[i].ports[j].ID)
                             .arg(this->block.boards[i].ports[j].num)
                             .arg(this->block.boards[i].ID)
                             .arg(this->block.boards[i].ports[j].media)
                             .arg(this->block.boards[i].ports[j].signal);
                    }
                    else{
                        query="UPDATE Port SET Num=%1, Id_board=%2, Media=%3, Signal=%4 WHERE Id=%5;";
                        query=query.arg(this->block.boards[i].ports[j].num)
                             .arg(this->block.boards[i].ID)
                             .arg(this->block.boards[i].ports[j].media)
                             .arg(this->block.boards[i].ports[j].signal)
                             .arg(this->block.boards[i].ports[j].ID);
                    }
                    if(!upPort.exec(query)){
                        std::wcout<<QString("Ошибка обновления данных: ").toStdWString().c_str()<<upPort.lastError().text().toStdWString().c_str()<<std::endl;
                    }
                    else
                        std::wcout<<QString("Данные по Port c №").toStdWString().c_str()<<this->block.boards[i].ports[j].num<<QString(" обновлены.").toStdWString().c_str()<<std::endl;
                }
            }
            }
        }
        db.close();
        std::wcout<<QString("Информация о блоке ").toStdWString().c_str()<<this->block.name.toStdWString().c_str()<<" c IP:"<<this->block.IP.toStdWString().c_str()<<QString(" обработана.").toStdWString().c_str()<<std::endl;
    }
    else
        std::cout <<"Неудалось поключиться к базе данных. Ошибка:"<<db.lastError().text().toStdString().c_str()<<std::endl;
    }
}
