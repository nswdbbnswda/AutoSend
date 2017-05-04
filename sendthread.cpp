
#include<QDebug>
#include<QHostAddress>
#include<iostream>
#include<QFile>
#include<vector>
#include"sendthread.h"




SendThread::SendThread()
{

}

SendThread::~SendThread()
{

}



void SendThread::sendFile()
{

}




void SendThread::run()
{




qDebug()<<"hello,this is new thread!";


       exec();       //进入事件循环

}


