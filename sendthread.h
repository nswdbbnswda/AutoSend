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
     explicit  SendThread(int socketDescriptor);
     virtual  ~SendThread();
     void run();
 private:
      Sender *sender;
      myTcpSocket *tcpSock;
      std::queue<QString> queueSend;
      int socketDescriptor;
 private slots:
       void pushFileQueue(QString fileName);//当有新的文件到来的时候把文件插入到队列中
        void Show();

};

#endif // SENDTHREAD_H
