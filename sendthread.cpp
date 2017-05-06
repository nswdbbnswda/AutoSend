
#include<QDebug>
#include<QHostAddress>
#include<iostream>
#include<QFile>
#include<vector>
#include"sendthread.h"





SendThread::SendThread(myTcpSocket *tcpSocket) :tcpSock(tcpSocket)/* :socketSend(socket),queueSend(queue)*/
{
//qDebug()<<tcpServer;
//qDebug()<<queue;

}




SendThread::~SendThread()
{

}








void SendThread::run()
{


//qDebug()<<"hello,this is new thread!";
//sender = new Sender(socketSend,queueSend);
//sender->sendFile();


//if(sender)
//delete sender;
exec();       //进入事件循环

}


