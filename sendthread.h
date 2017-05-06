#ifndef SENDTHREAD_H
#define SENDTHREAD_H
#include<QThread>
#include<QDebug>
#include<QTcpSocket>
#include<queue>
#include"sender.h"
#include<QTcpServer>
#include"mytcpsocket.h"





class SendThread : public QThread
{    Q_OBJECT
 public:
     explicit  SendThread(myTcpSocket *tcpSocket);
     virtual  ~SendThread();
     void run();
 private:
      Sender *sender;
      myTcpSocket *tcpSock;
      std::queue<QString> *queueSend;

};

#endif // SENDTHREAD_H
