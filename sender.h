#ifndef SENDER_H
#define SENDER_H
#include<QObject>
#include<QTcpSocket>
#include<queue>
#include<QDebug>
#include<QFile>
#include<string>
#include<QTime>
#include<stdio.h>
#include<iostream>
#include"pathremake.h"
#include<QVariant>
#include<QSharedMemory>



class Sender : public QObject
{
    Q_OBJECT
public:
    explicit Sender(int _port);
    virtual ~Sender();
    void    sendFile(std::queue<QString> &curfileQueue,qint64 pos = 0);//发送文件
    void    sendTaskCode();//发送文件清单
    void    setSocket( QTcpSocket *socket);//设置发送套接字
private:
    QTcpSocket                *m_Socket;//用于发送的套接字
    std::queue<QString>       fileQueue;//备份文件队列
    std::map<QString,QString> nametoFullPath;//将接收到的断点文件转换成全路径文件的字典
    std::map<QString,QString> fullPathtoName;//路径到文件名的转换
    std::queue<QString>       pathQueue;//要遍历的文件路径队列
    QTime   time;
    bool    finishFlag;
    bool    breakFlag;
    qint64  finishByte;//已经发送的总字节数
    qint64  cunrrentFinishByte;//当前发送的字节数
    qint64  FileLength;//文件长度
    QString memId;//共享内存ID
    QSharedMemory *printMem;
    char*   shareBuffer;//共享内存buffer指针
private:
    bool    adjustedQueues(const QByteArray &fileName ,std::queue<QString> &fileQue);//调整文件队列到断点时状态
    unsigned long nameHash(std::queue<QString> fileNameQue);//求任务编号
public slots:
    void    LostConnection();
    void    showSpeed();//显示进度
    void    acceptRequest();//接受并处理客户端的请求
signals:
    void    refresh(qint64);//刷新进度
    void    finishSend();//发送完成
};

#endif // SENDER_H
