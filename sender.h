#ifndef SENDER_H
#define SENDER_H
#include<QObject>
#include<QTcpSocket>
#include<queue>
#include<QDebug>
#include<QFile>
#include<string>


class Sender : public QObject
{
    Q_OBJECT
public:
    explicit Sender( QTcpSocket *socket, std::queue<QString> *queue);
    virtual ~Sender();
    void sendFile();//发送文件
private:
    QTcpSocket *m_Socket;//用于发送的套接字
    std::queue<QString> *Fileque;//待发送的文件队列



};

#endif // SENDER_H
