#include "server.h"
#include<QHostAddress>
#include<iostream>
#include<QFile>
#include<vector>
#include"sendthread.h"
#include<Windows.h>


std::string Server::dirpath;//定义静态变量
Server::Server():ThreadNum(0)

{
   //路径初始化
    std::cout<<"path:";
    std::cin>>dirpath;

    //f1 = new  FileWatcher(dirpath);//启动监视器
    //f1->watchEverything();
    FileWatcher::getInstance(dirpath)->watchEverything();//启动监视器

   //TCP
     server = new MyTcpSever;
    if (!server->isListening()){//监听
        if (server->listen(QHostAddress::Any, 5555)){
            std::cout<<"open listen port success!"<<std::endl;
        }
        else{ std::cout<<"open listen port fail!"<<std::endl;}
    }
    else{std::cout<<"Function  isListening()  error!"<<std::endl; }
    connect(server, SIGNAL(newClientConnection(qintptr)), this, SLOT(newConnectionSlot(qintptr)));//当有新的连接的时候，就会执行槽函数
    qDebug()<<QThread::currentThreadId();

}



Server::~Server(){
    if(server) {
        delete server;
        server = NULL;
    }
    if(m_Socket){
        delete[] m_Socket;
    }


    if(sendThread1){
        delete[] sendThread1;
    }

    delete FileWatcher::getInstance(Server::dirpath);//释放监视器


}

void Server::newConnectionSlot(qintptr ptr1)
{
  if(ThreadNum<Min){
    sendThread1[ThreadNum] = new SendThread(ptr1);//把这个发送套接字传给工作线程
    sendThread1[ThreadNum]->moveToThread(sendThread1[ThreadNum]);//把这个对象移动到子线程中去，让sendThread1[ThreadNum]对象的槽函数都属于依附子线程

    sendThread1[ThreadNum]->start();//启动子线程
    ++ThreadNum;
  }
}












