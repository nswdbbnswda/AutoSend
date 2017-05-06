#include "server.h"
#include<QHostAddress>
#include<iostream>
#include<QFile>
#include<vector>
#include"sendthread.h"
#include<Windows.h>



Server::Server()

{
   //路径初始化
    std::cout<<"path:";
    std::cin>>dirpath;

    f1 = new  FileWatcher(dirpath);
    f1->watchEverything();

   //TCP
   server = new QTcpServer(this);//创建TCP套接字
    if (!server->isListening()){//监听
        if (server->listen(QHostAddress::Any, 5555)){
            std::cout<<"open listen port success!"<<std::endl;
        }
        else{ std::cout<<"open listen port fail!"<<std::endl;}
    }
    else{std::cout<<"Function  isListening()  error!"<<std::endl; }
   connect(server, SIGNAL(newConnection()), this, SLOT(newConnectionSlot()));//当有新的连接的时候，就会执行槽函数



//   int x = 111;
//   auto f =[x](int a,int b){return a+b+x;};

   //qDebug()<<f(1,1);

}



Server::~Server(){
    if(server) {
        delete server;
        server = NULL;
    }
    if(m_Socket){
        delete[] m_Socket;
    }

if(f1){
    delete f1;
}


    if(sendThread){
        delete[] sendThread;
    }
}




//当有新的连接的时候为这个连接创建一个线程然后把套接字传给这个线程里面的发送器进行发送操作
void Server::newConnectionSlot(){


//    FileWatcher f1 =  FileWatcher(dirpath);
//    f1.watchEverything();

    if(ThreadNum<Min){
    m_Socket[ThreadNum] = server->nextPendingConnection();//为新的连接创建一个发送套接字
//   qDebug()<<m_Socket[ThreadNum];
//   qDebug()<<&FileWatcher::fileQueue;
   // qDebug()<<server->socketDescriptor();
    // sendThread[ThreadNum] = new SendThread( server->socketDescriptor(),&FileWatcher::fileQueue);//创建线程
     //sendThread[ThreadNum]->start();//启动线程
       fuck = new Sender(m_Socket[ThreadNum],&FileWatcher::fileQueue);
      connect(f1,SIGNAL(queueNonEmpty()),this,SLOT(newSendFile()));


        fuck->sendFile();

      //  qDebug()<<"quit send function";
       // qDebug()<<"out of sendFile Fun";
      // ++ThreadNum;
    // qDebug()<<ThreadNum;
  }



}

void Server::newSendFile()
{
    fuck->sendFile();
}







