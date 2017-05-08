#ifndef MYTCPSOCKET_H
#define MYTCPSOCKET_H
#include<QTcpSocket>
#include<QDebug>


class myTcpSocket : public QTcpSocket
{Q_OBJECT
public:
    explicit myTcpSocket();
    virtual ~myTcpSocket();
};

#endif // MYTCPSOCKET_H
