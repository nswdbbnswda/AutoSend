#ifndef SERVER_H
#define SERVER_H
#include<QTcpServer>
#include<QTcpSocket>
#include<QObject>
#include<iostream>
#include<string>
#include<QFile>
#include<QDir>
#include<QFile>
#include<QDebug>
#include<QDateTime>
#include<QDirIterator>
#include<queue>
#include<QFileSystemWatcher>
#include"filewatcher.h"
#include"sender.h"
#include"mytcpsever.h"
#include<QSharedMemory>


class Server : QObject
{  Q_OBJECT
public:
    explicit Server(const std::string &_strPort);
    virtual ~Server();
private:
    MyTcpSever *server;
    QTcpSocket *m_Socket;//连接套接字
    std::string path;
    qint64 ThreadNum;
    qint32 m_port;
    Sender *sender;//发送器
    QTcpSocket *tcpSock;//发送文件的套接字
    QSharedMemory *sharememory;
private:
    void WriteDataToMem(char c);//向共享内存中写数据
public:
    static std::string dirpath;//声明
private slots:
    void newConnectionSlot(qintptr ptr1);//对新的TCP连接进行处理
public slots:
    void quitAutoSend();//退出程序

};

#endif // SERVER_H
