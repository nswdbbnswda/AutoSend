
#include<QDebug>
#include<QHostAddress>
#include<iostream>
#include<QFile>
#include<vector>
#include"sendthread.h"
#include"filewatcher.h"
#include<string>
#include"server.h"
#include <QCoreApplication>

SendThread::SendThread(int socketDescriptor)
{
     this ->socketDescriptor = socketDescriptor;//设置套接字描述符
}


SendThread::~SendThread()
{

    if(sender){
        delete sender;
        sender = NULL;
    }

    if(tcpSock){
        delete  tcpSock;
        tcpSock = NULL;

    }

}


void SendThread::run()
{

tcpSock = new myTcpSocket;//创建一个套接字
tcpSock->setSocketDescriptor(socketDescriptor);//为这个套接字设置套接字描述符
//connect(tcpSock,SIGNAL(disconnected()),this,SLOT(deleteLater()));//如果断开连接了，清理资源  //测试结果是加了这两句反而内存会变化。
//connect(tcpSock,SIGNAL(disconnected()),this,SLOT(terminate()));//如果断开连接了，就终止该线程
//使用监视器
FileWatcher::getInstance(Server::dirpath)->GetFileList(QString::fromStdString(Server::dirpath),queueSend);//通过监视器获得文件列表
//连接监视器，当有新文件时，把文件名字插入到队列，然后发送出去
//connect(FileWatcher::getInstance(Server::dirpath),SIGNAL(fileChange(QString)),SLOT(pushFileQueue(QString)));
//创建一个发送器
sender = new Sender(tcpSock,&queueSend);
sender->sendFile();//发送文件

//QCoreApplication::exit();

exec();       //进入事件循环

}


//当队列中有新的文件被添加进来后，触发这个槽函数,重新启动文件发射器
void SendThread::pushFileQueue(QString fileName)
{

    //qDebug()<<QThread::currentThreadId();
    queueSend.push(fileName);
    sender->sendFile();
}

void SendThread::Show()
{
    qDebug()<<"disconnected!";
}



