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


    FileWatcher f1(dirpath);
    f1.watchEverything();

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
}



Server::~Server(){
    if(server) {
        delete server;
        server = NULL;
    }
    if(m_Socket){
        delete[] m_Socket;
    }




    if(sendThread){
        delete[] sendThread;
    }
}



//当有新的连接的时候为这个连接创建一个线程然后把套接字传给这个线程里面的发送器进行发送操作
void Server::newConnectionSlot(){

    if(ThreadNum<Min){
     m_Socket[ThreadNum] = server->nextPendingConnection();//为新的连接创建一个发送套接字
      sendThread[ThreadNum] = new SendThread;//创建线程
      sendThread[ThreadNum]->start();//启动线程
        Sender fuck(m_Socket[ThreadNum],&FileWatcher::fileQueue);
        fuck.sendFile();
      ++ThreadNum;
      qDebug()<<ThreadNum;
  }

}







