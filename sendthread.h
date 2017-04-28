#ifndef SENDTHREAD_H
#define SENDTHREAD_H
#include<QThread>


class SendThread : public QThread
{    Q_OBJECT
 public:
     explicit  SendThread();
     virtual  ~SendThread();
     void run();


};

#endif // SENDTHREAD_H
