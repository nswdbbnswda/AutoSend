#ifndef SENDTHREAD_H
#define SENDTHREAD_H
#include<QThread>
#include<QDebug>





class SendThread : public QThread
{    Q_OBJECT
 public:
     explicit  SendThread();
     virtual  ~SendThread();
     void sendFile();//发送文件
     void run();





};

#endif // SENDTHREAD_H
