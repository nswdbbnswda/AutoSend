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


class Sender : public QObject
{
    Q_OBJECT
public:
    explicit Sender( QTcpSocket *socket, std::queue<QString> *queue);
    Sender() = default;
    virtual ~Sender();
    void sendFile();//发送文件
private:
    QTcpSocket *m_Socket;//用于发送的套接字
    std::queue<QString> *Fileque;//待发送的文件队列
    QTime time;
    bool finishFlag;
    qint64 finishByte;//已经发送的总字节数
    qint64 cunrrentFinishByte;//当前发送的字节数
    qint64 FileLength;//文件长度

public slots:
    void LostConnection();
    void showSpeed(qint64);//显示进度
signals:
    void refresh(qint64);//刷新进度
    void finishSend();//发送完成


};

#endif // SENDER_H
