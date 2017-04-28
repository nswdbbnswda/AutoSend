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



class Server : QObject
{  Q_OBJECT
public:
    explicit Server();
    virtual ~Server();
private:
    QTcpServer *server;//监听套接字
    QTcpSocket *m_Socket;//连接套接字
    std::string path,dirpath;
    qint32 FileLength;//文件的长度
    qint32 BlockNum;//整块数量
    qint32 TotalByte,temp,PathLength;
    qint32 Num2,Num3,Num4,TotalNum;
    qint32 LastBlock;
    char  *SendBuffer;
    char  *SendPath;
    qint32 lenth;
    char *FileNum;
    SendThread work;
    std::map<QString,size_t> mymapCur;
    std::map<QString,size_t> mymapLast;
    std::queue<QString> *Fileque;////文件队列
    std::queue<QString> *Pathque;//路径队列
    QFileSystemWatcher myWatcher;
    SendThread sendThread;//发送线程
private:
    void  GetFileList(const QString &path);//获得目录下的所有文件
    void WatchEvery(const QString &path);
    void SendFile();

signals:
    void Send();//发送数据
private slots:
    void newConnectionSlot();//对新的TCP连接进行处理
    void showMessage(const QString &path);
    void sendChange();//当文件改变的时候触发这个槽

};

#endif // SERVER_H
