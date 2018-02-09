#include "server.h"
#include<QHostAddress>
#include<iostream>
#include<QFile>
#include<vector>
#include<Windows.h>
#include"pathremake.h"

std::string Server::dirpath;//定义静态变量
Server::Server(const std::string &_strPort)
{
    ThreadNum = 0;
    m_port = atoi(_strPort.data());//把端口号从string 转换成整数类型

    //TCP
    server = new MyTcpSever;
    tcpSock = new QTcpSocket;//创建一个套接字
    sender = new Sender(m_port);//创建一个文件发送器



    sharememory = new QSharedMemory();//构造实例对象
    sharememory->setKey("AutoSend");//找到内存中的共享内存



    if (!server->isListening()){//监听
        if (server->listen(QHostAddress::Any,m_port)){
           // std::cout<<"open listen port success!"<<std::endl;//提示监听成功
            WriteDataToMem('1');
        }
        else{
            std::cout<<"open listen port fail!"<<std::endl;
            WriteDataToMem('0');
            exit(0);
        }//提示监听失败
    }
    else{
        std::cout<<"Function  isListening()  error!"<<std::endl;
        WriteDataToMem('0');
        exit(0);
    }
    connect(server, SIGNAL(newClientConnection(qintptr)), this, SLOT(newConnectionSlot(qintptr)));//当有新的连接的时候，就会执行槽函数
    connect(sender,SIGNAL(finishSend()),this,SLOT(quitAutoSend()));//接收到了sender的发送完毕信号就退出程序.
}


Server::~Server()
{   if(sharememory) delete sharememory;
    if(server) {delete server;server = NULL;}
    if(m_Socket){ delete m_Socket;}
    if(tcpSock) delete tcpSock;
    if(sender)  delete sender;
    delete FileWatcher::getInstance();//释放监视器
}

//向共享内存中写数据
bool Server::WriteDataToMem(char c)
{
    //取得共享内存权限
    sharememory->lock();
    sharememory->attach();//使该进程可以访问共享内存数据
    if(!sharememory->isAttached()) return false;//如果没有关联上共享内存就直接返回false
    //读取数据
    *((char*)sharememory->data()) = c;
    sharememory->unlock();
    sharememory->detach();//使本进程与共享内存分离
}


//当有新的连接的时候响应这个函数
void Server::newConnectionSlot(qintptr ptr1)
{
    if(!tcpSock->setSocketDescriptor(ptr1)){//为这个套接字设置套接字描述符
        qDebug()<<"setSocketDescriptor failed!";
    }
    sender->setSocket(tcpSock);//设置发送套接字
    sender->sendTaskCode();//发送任务代号
}


//退出程序
void Server::quitAutoSend()
{
    exit(0);
}












