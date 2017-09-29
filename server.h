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



class Server : QObject
{  Q_OBJECT
public:
    explicit Server(const char *inputPort);
    virtual ~Server();
private:
    MyTcpSever *server;
    QTcpSocket *m_Socket;//连接套接字
    std::string path;
    qint64 ThreadNum;
    FileWatcher *f1;
    qint32 m_iPort;
    Sender *sender;//发送器
    QTcpSocket *tcpSock;//发送文件的套接字
    std::queue<QString> queueSend;//文件队列

public:
    static std::string dirpath;//声明
signals:

private slots:
    void newConnectionSlot(qintptr ptr1);//对新的TCP连接进行处理
public slots:
    void quitAutoSend();//退出程序

};

#endif // SERVER_H
