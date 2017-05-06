#ifndef MYTCPSOCKET_H
#define MYTCPSOCKET_H
#include<QTcpSocket>


class myTcpSocket : public QTcpSocket
{Q_OBJECT
public:
    explicit myTcpSocket(qintptr socketDescriptor);
    virtual ~myTcpSocket();
};

#endif // MYTCPSOCKET_H
