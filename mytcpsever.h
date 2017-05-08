#ifndef MYTCPSEVER_H
#define MYTCPSEVER_H
#include<QTcpServer>
#include"mytcpsocket.h"
#include"sendthread.h"


class MyTcpSever : public QTcpServer
{ Q_OBJECT
public:
    explicit MyTcpSever();
    virtual ~MyTcpSever();
protected:
    void incomingConnection(qintptr socketDescriptor);
private:
  //  SendThread *thread;
signals:
    void newClientConnection(qintptr socketDescriptor);



};

#endif // MYTCPSEVER_H
