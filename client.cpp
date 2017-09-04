#include "client.h"
#include<QHostAddress>
#include<QDebug>
#include <QCoreApplication>
//启动客户端后就开始连接服务端
Client::Client(const std::string strIpAddr,const std::string  inputPort):NameLength(0),ReceiveName(NULL),m_pSocket(NULL),ReceiveFileNum(NULL),num(0),CurrentNum(0),TotalNum(0),
    LastBlock(0),Flag(0),TotalByte(0),WrittenByte(0),FileNumber(0) ,ipAddr(strIpAddr)
{
    finishFlag = false;
    logFile = new QFile(QCoreApplication::applicationDirPath()+"/RecvLog.txt");//建立日志文件
    logFile->open(QIODevice::WriteOnly | QIODevice::Append);

    port = atoi(inputPort.c_str());//把端口号从string 类型转换成 整型
    m_pSocket = new QTcpSocket();//创建客户端套接字
   connectToServer();//连接服务器

    connect(m_pSocket,SIGNAL(disconnected()),this,SLOT(LostConnection()));//断开连接了退出

   //连接信号和槽
    connect(this, SIGNAL(DataComing()),this, SLOT(ReceiveData()));
    emit DataComing();//发送信号，文件来了
    connect(m_pSocket,SIGNAL(connected()),this,SLOT(LostConnection()));//断开连接了退出

}



Client::~Client(){
    if(m_pSocket) {
        delete m_pSocket;
        m_pSocket = NULL;
    }
    if(logFile){
        delete logFile;
        logFile = NULL;
    }
}


//接收数据
void Client::ReceiveData(){

        //等待发送端把文件的个个数发送的过来
       QByteArray  fileNum;
       qint64  totalFileNum = 0;

        while(m_pSocket->bytesAvailable()<8){
            m_pSocket->waitForReadyRead();
        }

        fileNum = m_pSocket->read(8);//读8个字节的文件数量
        ReceiveFileNum = fileNum.data();
        memcpy(&totalFileNum,ReceiveFileNum,8);
        qDebug()<<totalFileNum;




        //循环接收每一个文件
        while(totalFileNum)
        {

        //if(m_pSocket->disconnect()){  std::cout<<"DIS"<<std::endl;}
        m_pSocket->waitForReadyRead();//先等一会直到有数据过来
       // while(m_pSocket->bytesAvailable())//有数据可读的时候才进来


            while(m_pSocket->bytesAvailable()<4){
                m_pSocket->waitForReadyRead();
            }
            vTemp=m_pSocket->read(4);//读4个字节，储存的是文件名字的字节数量
            ReceiveName=vTemp.data();
            memcpy(&NameLength,ReceiveName,4);//NameLength储存的是文件名字所占的字节数量
            while(m_pSocket->bytesAvailable()<NameLength){
                m_pSocket->waitForReadyRead();
            }
            vTemp=m_pSocket->read(NameLength);//读文件名字  读NameLength个字节
            vTemp.remove(0,1);//删除第一个字节（盘符）
            vTemp.insert(0,QByteArray("D"));//更改为E盘符
            //搞定路径
            QString fullPath(vTemp);//QByteArray 转换成 QString
            KoPath(fullPath);//搞定路径问题,如果没有则创建

            qDebug()<<fullPath;
            //在硬盘上建立该文件
            QFile file(vTemp.data());          
            file.open(QFile::WriteOnly);//只写方式打开
            logFile->write(vTemp); //写入日志文件
            logFile->write("\r\n");


            //读12个字节的文件头信息
            while(m_pSocket->bytesAvailable()<12){
                m_pSocket->waitForReadyRead();
            }
            vTemp=m_pSocket->read(12);//读12字节
            ReceiveHead=vTemp.data();//类型转换QByteArray到char *
            memcpy(&CurrentNum,ReceiveHead,4);
            memcpy(&TotalNum,&ReceiveHead[4],4);//整个文件的包的数量
            memcpy(&LastBlock,&ReceiveHead[8],4);//当前文件最后一个包的字节数
            TotalByte=(TotalNum-1)*8388608+LastBlock;//当前文件的总的字节数量

            if(TotalNum>1){
                while(TotalNum-1){
                    while(m_pSocket->bytesAvailable()<8388608){
                        m_pSocket->waitForReadyRead();
                    }
                    vTemp = m_pSocket->read(8388608); //读取838608个字节
                    //vTemp.clear();
                    while(m_pSocket->bytesAvailable()<12){
                        m_pSocket->waitForReadyRead();
                    }
                    xTemp=m_pSocket->read(12);
                    xTemp.clear();
                    WrittenByte += file.write(vTemp);//写入数据
                    TotalNum--;
                    //存在一个BUG 尚未进行对12字节处理
                }
                while(m_pSocket->bytesAvailable()<LastBlock){
                    m_pSocket->waitForReadyRead();
                }
                vTemp=m_pSocket->read(LastBlock);//第一次读取读最后一块数据
                WrittenByte += file.write(vTemp);
                // vTemp.clear();
            }

            else{//处理最后一个数据块
                while(m_pSocket->bytesAvailable()<LastBlock){
                    m_pSocket->waitForReadyRead();
                }
                vTemp=m_pSocket->read(LastBlock);//第一次读取读最后一块数据
                WrittenByte += file.write(vTemp);
                // vTemp.clear();
                //emit  DataWritten();
            }

            file.close();//完成一个文件的读写

           // m_pSocket->waitForReadyRead();//等待下一个文件的数据流到来
              totalFileNum--;
        }

       // exit(0);//如果一定时间内都没有数据可以读了,那么就认为服务端不再有数据进行发送了，客户端就可以退出了



         logFile->close();//关闭日志文件
        qDebug()<<"OK";
        finishFlag = true;
        m_pSocket->close();//关闭套接字
}



//打印"进度条"
void  Client::UpProgress(){
    std::cout<<">";
}



void Client::LostConnection(){
   if(finishFlag) exit(0);//发送完毕，退出程序
    else{
        qDebug()<<"Lost connection!";
        qDebug()<<"Reconnecting......";
        connectToServer();//重新连接
        qDebug()<<"The network connection has been restored!";
   }
}






//输入:文件全路径，输出:在该路径上创建文件
bool  Client::KoPath(const QString &dirName)//文件全路径(包含文件名）
{
    QString fullPath;
    QFileInfo fileInfo(dirName);
    fullPath = fileInfo.absolutePath();//全路径，不包括文件名
    QDir dir(fullPath);//创建目录对象
    if(dir.exists()){
        return true;
    }
    else{
        bool ok = dir.mkpath(fullPath);//创建多级目录
        return ok;
    }
}

//发起TCP连接
void Client::connectToServer(){
    while(true){
     m_pSocket->connectToHost(QHostAddress(ipAddr.c_str()),port);//发起连接
     if(m_pSocket->waitForConnected()) break;//如果连上了服务器，函数返回
    }
}






