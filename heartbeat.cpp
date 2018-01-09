#include "heartbeat.h"

HeartBeat::HeartBeat()
{
}

HeartBeat::~HeartBeat()
{
    if(myTimer) delete myTimer;
    if(lock)    delete lock;
    if(pSocket) delete pSocket;
}


void HeartBeat::Burn(int desc)
{
   pSocket = new QTcpSocket;
   pSocket->setSocketDescriptor(desc);

   myTimer = new QTimer;
   lock = new QMutex;

   connect(myTimer,SIGNAL(timeout()),this,SLOT(heartTime()));

}

//每个5秒触发一次（心跳机制）
void HeartBeat::heartTime()
{
    lock->lock();
    if(-1 == pSocket->write("?") || !pSocket->waitForBytesWritten()){
       pSocket->disconnectFromHost();
        myTimer->stop();//断线了就关闭定时器
    }
    lock->unlock();
  //  pSocket->flush();

}

//启动定时器
void HeartBeat::startTimer()
{
    myTimer->start(5000);//启动定时器
}
