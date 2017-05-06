#include "mytcpsever.h"

MyTcpSever::MyTcpSever()
{

}

MyTcpSever::~MyTcpSever()
{

}

void MyTcpSever::incomingConnection(qintptr socketDescriptor)
{
myTcpSocket *tcptemp = new myTcpSocket(socketDescriptor);//通过套接字描述符创建一个临时的发送套接字
SendThread *thread = new SendThread(tcptemp);//把这个发送套接字传给工作线程

}
