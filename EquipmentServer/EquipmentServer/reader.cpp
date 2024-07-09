#include "reader.h"
#include <QDomDocument>
#include <QFile>
#include <iostream>
#include "board.h"
#include "port.h"
#include "global.h"
#include <QString>
//Block block;
Reader::Reader(QObject *parent, QString fn):QThread(parent)
{
   this->fileName=fn;
}
void Reader::run(){
    QFile data(this->fileName);
    data.open(QIODevice::ReadOnly);
    if(data.isOpen()){
          QDomDocument doc;
          QString error;
          int r, c;
          if(doc.setContent(&data, true, &error, &r, &c)){
          data.close();
          QDomElement root=doc.documentElement();
          QDomNodeList blocks=root.childNodes();
          if(blocks.at(0).nodeName()!=QString("block") || blocks.count()!=1){
              block.name="error";
              std::wcout<<this->fileName.toStdWString()<<QString(" - это XML не описывает оборудование.").toStdWString().c_str()<<std::endl;
              return;}
          for(int j=0; j<blocks.at(0).toElement().attributes().count(); j++){
              if(blocks.at(0).toElement().attributes().item(j).toAttr().name()=="id"){
                 block.ID=blocks.at(0).toElement().attributes().item(j).toAttr().value().toLong();
              }
              if(blocks.at(0).toElement().attributes().item(j).toAttr().name()=="Name"){
                  block.name=blocks.at(0).toElement().attributes().item(j).toAttr().value();
              }
              if(blocks.at(0).toElement().attributes().item(j).toAttr().name()=="IP"){
                  block.IP=blocks.at(0).toElement().attributes().item(j).toAttr().value();
              }
              if(blocks.at(0).toElement().attributes().item(j).toAttr().name()=="BoardCount"){
                  block.boardCount=blocks.at(0).toElement().attributes().item(j).toAttr().value().toLong();
              }
              if(blocks.at(0).toElement().attributes().item(j).toAttr().name()=="MtR"){
                  block.MtR=blocks.at(0).toElement().attributes().item(j).toAttr().value().toLong();
              }
              if(blocks.at(0).toElement().attributes().item(j).toAttr().name()=="MtC"){
                  block.MtC=blocks.at(0).toElement().attributes().item(j).toAttr().value().toLong();
              }
              if(blocks.at(0).toElement().attributes().item(j).toAttr().name()=="Description"){
                  block.description=blocks.at(0).toElement().attributes().item(j).toAttr().value();
              }
              if(blocks.at(0).toElement().attributes().item(j).toAttr().name()=="Label"){
                 block.label=blocks.at(0).toElement().attributes().item(j).toAttr().value();
              }
           }
          QDomNodeList boards=blocks.at(0).childNodes();
          for(int j=0; j<boards.count(); j++){
              if(boards.at(j).nodeName()!=QString("board")){
                  block.name="error";
                  std::wcout<<this->fileName.toStdWString()<<QString(" - это XML не описывает оборудование.").toStdWString().c_str()<<std::endl;
                  return;
              }
              Board brd;
              for(int k=0; k<boards.at(j).toElement().attributes().count(); k++){
                  if(boards.at(j).toElement().attributes().item(k).toAttr().name()=="id"){
                      brd.ID=boards.at(j).toElement().attributes().item(k).toAttr().value().toLong();
                  }
                  if(boards.at(j).toElement().attributes().item(k).toAttr().name()=="Name"){
                      brd.name=boards.at(j).toElement().attributes().item(k).toAttr().value();
                  }
                  if(boards.at(j).toElement().attributes().item(k).toAttr().name()=="Num"){
                      brd.num=boards.at(j).toElement().attributes().item(k).toAttr().value().toLong();
                  }
                  if(boards.at(j).toElement().attributes().item(k).toAttr().name()=="PortCount"){
                      brd.portCount=boards.at(j).toElement().attributes().item(k).toAttr().value().toLong();
                  }
                  if(boards.at(j).toElement().attributes().item(k).toAttr().name()=="IntLinks"){
                      brd.intLinks=boards.at(j).toElement().attributes().item(k).toAttr().value();
                  }
                  if(boards.at(j).toElement().attributes().item(k).toAttr().name()=="Algoritms"){
                      brd.Algoritms=boards.at(j).toElement().attributes().item(k).toAttr().value();
                  }
              }
              QDomNodeList ports=boards.at(j).childNodes();
              for(int k=0; k<ports.count(); k++){
                  if(ports.at(k).nodeName()!=QString("port")){
                      block.name="error";
                      std::wcout<<this->fileName.toStdWString()<<QString(" - это XML не описывает оборудование.").toStdWString().c_str()<<std::endl;
                      return;
                  }
                  Port p;
                  for(int w=0; w<ports.at(k).toElement().attributes().count(); w++){
                      if(ports.at(k).toElement().attributes().item(w).toAttr().name()=="id"){
                          p.ID=ports.at(k).toElement().attributes().item(w).toAttr().value().toLong();
                      }
                      if(ports.at(k).toElement().attributes().item(w).toAttr().name()=="Num"){
                          p.num=ports.at(k).toElement().attributes().item(w).toAttr().value().toLong();
                      }
                      if(ports.at(k).toElement().attributes().item(w).toAttr().name()=="Media"){
                          p.media=ports.at(k).toElement().attributes().item(w).toAttr().value().toLong();
                      }
                      if(ports.at(k).toElement().attributes().item(w).toAttr().name()=="Signal"){
                          p.signal=ports.at(k).toElement().attributes().item(w).toAttr().value().toLong();
                      }
                  }
                  brd.ports.push_back(p);
               }
              block.boards.push_back(brd);
          }
          }
          else{
              std::wcout<<QString("Ошибка в документе ").toStdWString().c_str()<<this->fileName.toStdWString()<<QString(" строка: ").toStdWString().c_str()<<r<<QString(" столбец: ").toStdWString().c_str()<<c<<" "<<error.toStdWString()<<std::endl;
              data.close();
              block.name="error";
          }
    }
    else
        std::wcout <<"Не удалось прочитать файл: "<<this->fileName.toStdWString();
}
