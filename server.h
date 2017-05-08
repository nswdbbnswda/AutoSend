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
#include"sendthread.h"
#include"filewatcher.h"
#include"sender.h"
#include"mytcpsever.h"


class Server : QObject
{  Q_OBJECT
public:
    explicit Server();
    virtual ~Server();
private:
    enum ThreadMaxNum{Min = 10,Mind = 20,Max = 30,Super = 1000};
    //QTcpServer *server;//监听套接字
    MyTcpSever *server;
    QTcpSocket *m_Socket[Min];//连接套接字
    std::string path;
   // std::string dirpath;
    SendThread *sendThread1[Min];
    qint64 ThreadNum;
    FileWatcher *f1;
public:
    static std::string dirpath;//声明
signals:

private slots:
     void newConnectionSlot(qintptr ptr1);//对新的TCP连接进行处理
public slots:



};

#endif // SERVER_H
