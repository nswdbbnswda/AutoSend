#include "mytcpsever.h"

MyTcpSever::MyTcpSever()
{

}





MyTcpSever::~MyTcpSever()
{

//    if(thread){
//    delete thread;
//    }
}




void MyTcpSever::incomingConnection(qintptr socketDescriptor)
{


//thread = new SendThread(socketDescriptor);//把这个发送套接字传给工作线程
//thread->start();//启动子线程
    emit newClientConnection(socketDescriptor);

}
