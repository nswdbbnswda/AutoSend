#include "server.h"
#include<QHostAddress>
#include<iostream>
#include<QFile>
#include<vector>
#include"sendthread.h"
#include<Windows.h>



std::string Server::dirpath;//定义静态变量
Server::Server(const char *inputPort)
{
    ThreadNum = 0;
    m_iPort = atoi(inputPort);//把端口号从string 转换成整数类型
    FileWatcher::getInstance(dirpath);//启动文件遍历器

   //TCP
     server = new MyTcpSever;
     tcpSock = new myTcpSocket;//创建一个套接字
     sender = new Sender;//创建一个文件发送器
    if (!server->isListening()){//监听
        if (server->listen(QHostAddress::Any, m_iPort)){
            std::cout<<"open listen port success!"<<std::endl;
        }
        else{ std::cout<<"open listen port fail!"<<std::endl;}
    }
    else{std::cout<<"Function  isListening()  error!"<<std::endl; }
    connect(server, SIGNAL(newClientConnection(qintptr)), this, SLOT(newConnectionSlot(qintptr)));//当有新的连接的时候，就会执行槽函数
    //qDebug()<<QThread::currentThreadId();

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
     if(tcpSock) delete tcpSock;
     if(sender)  delete sender;
    delete FileWatcher::getInstance(Server::dirpath);//释放监视器
}


//当有新的连接的时候响应这个函数
void Server::newConnectionSlot(qintptr ptr1){
 tcpSock->setSocketDescriptor(ptr1);//为这个套接字设置套接字描述符
 FileWatcher::getInstance(Server::dirpath)->GetFileList(QString::fromStdString(Server::dirpath),queueSend);//通过监视器获得文件列表

 sender = new Sender(tcpSock,&queueSend);
 connect(sender,SIGNAL(finishSend()),this,SLOT(quitAutoSend()));//接收到了sender的发送完毕信号就退出程序.
 sender->sendFile();//发送文件


}

//退出程序
void Server::quitAutoSend()
{
   // qDebug()<<"EXIT!";
    exit(0);//
}












