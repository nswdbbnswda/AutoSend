#include "client.h"
#include<QHostAddress>
#include<QDebug>
#include <QCoreApplication>
#include"autosend.h"
//启动客户端后就开始连接服务端
Client::Client(const std::string strIpAddr,const std::string  inputPort)
{
    nameLength = 0;
    receiveName = NULL;
    m_pSocket = NULL;
    receiveFileNum = NULL;
    num = 0;
    CurrentNum = 0;
    TotalNum = 0;
    LastBlock = 0;
    Flag = 0;
    FileLength = 0;
    FileNumber = 0;
    ipAddr = strIpAddr;
    finishByte = 0;
    cunrrentFinishByte = 0;
    finishFlag = false;
    logFile = new QFile(QCoreApplication::applicationDirPath()+"/RecvLog.txt");//建立日志文件
    logFile->open(QIODevice::WriteOnly | QIODevice::Append);

    port = atoi(inputPort.c_str());//把端口号从string 类型转换成 整型
    m_pSocket = new QTcpSocket();//创建客户端套接字
    m_pSocket->setReadBufferSize(IPMSG_DEFAULT_QTINERBUFFER);//设置QT内部缓冲区大小为10M
    //qDebug()<<m_pSocket->readBufferSize();//显示QT内部缓冲区大小
    connectToServer();//连接服务器
    connect(this,SIGNAL(refresh()),this,SLOT(showSpeed()));
    connect(m_pSocket,SIGNAL(disconnected()),this,SLOT(lostConnection()));//断开连接了做异常处理
   //连接信号和槽将数据接收并存到硬盘中
    connect(this, SIGNAL(DataComing()),this, SLOT(ReceiveData()));

    emit DataComing();//发送信号，文件来了
    //connect(m_pSocket,SIGNAL(readyRead()),this,SLOT(test()));//测试连接，当有数据过来的时候直接将数据全部读出来，然后再查看cpu占用率

}


Client::~Client()
{
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
void Client::ReceiveData()
{



    //等待发送端把文件的个个数发送的过来
       QByteArray  fileNum;
       qint64  totalFileNum = 0;

        while(m_pSocket->bytesAvailable()<8){//等待至少有8个字节数据到来
            if(!m_pSocket->waitForReadyRead()){
                 m_pSocket->disconnectFromHost();//如果超时了，尝试重新连接服务器

            }
        }

        fileNum = m_pSocket->read(8);//把这8个字节读到字节数组里
        receiveFileNum = fileNum.data();//转换成char *类型
        memcpy(&totalFileNum,receiveFileNum,8);//拷贝到totalFileNum变量中
        qDebug()<<totalFileNum;//显示有多少个文件

        //循环接收每一个文件
        while(totalFileNum){
            time.restart();
            finishByte = 0;

            if(!m_pSocket->waitForReadyRead()){
                 m_pSocket->disconnectFromHost();

            }
            while(m_pSocket->bytesAvailable()<4){//保证至少先读到储存文件名长度的变量

                if(!m_pSocket->waitForReadyRead()){

                     m_pSocket->disconnectFromHost();

                }
            }
            vTemp = m_pSocket->read(4);//读4个字节，储存的是文件名字的字节数量
            receiveName = vTemp.data();//转换成char*类型
            memcpy(&nameLength,receiveName,4);//nameLength储存的是文件名字所占的字节数量
            while(m_pSocket->bytesAvailable() < nameLength){//如果当前缓冲区字节数不足nameLength，就缓冲到足为止
                if(!m_pSocket->waitForReadyRead()){
                     m_pSocket->disconnectFromHost();
                }
            }

            vTemp = m_pSocket->read(nameLength);//读文件名字  读NameLength个字节
            vTemp.remove(0,1);//删除第一个字节（盘符）
            vTemp.insert(0,QByteArray("D"));//更改为D盘符
            //搞定路径
            QString fullPath(vTemp);//QByteArray 转换成 QString
            KoPath(fullPath);//搞定路径问题,如果没有则创建

            qDebug()<<fullPath;//显示当前传输的文件名称

            //在硬盘上建立该文件
            QFile file(vTemp.data());
            file.open(QFile::WriteOnly);//只写方式打开
            logFile->write(vTemp); //写入日志文件
            logFile->write("\r\n");//日志格式


            //读12个字节的文件头信息
            while(m_pSocket->bytesAvailable() < 12){

                if(!m_pSocket->waitForReadyRead()){
                    m_pSocket->disconnectFromHost();
                }

            }
            vTemp = m_pSocket->read(12);//读12字节
            ReceiveHead = vTemp.data();//类型转换QByteArray到char *
            memcpy(&CurrentNum,ReceiveHead,4);
            memcpy(&TotalNum,&ReceiveHead[4],4);//整个文件的包的数量
            memcpy(&LastBlock,&ReceiveHead[8],4);//当前文件最后一个包的字节数
            FileLength = (TotalNum-1) * IPMSG_DEFAULT_IOBUFMAX + LastBlock;//当前文件的总的字节数量
            //qDebug()<<totalByte;//显示文件的总字节数

            if(TotalNum>1){//如果存在整包

                while(TotalNum-1){//运行的次数
                    while(m_pSocket->bytesAvailable() < IPMSG_DEFAULT_IOBUFMAX){//缓冲IPMSG_DEFAULT_IOBUFMAX个字节数

                        if(!m_pSocket->waitForReadyRead()){
                             m_pSocket->disconnectFromHost();
                        }

                    }
                    vTemp = m_pSocket->read(IPMSG_DEFAULT_IOBUFMAX); //读取IPMSG_DEFAULT_IOBUFMAX个字节


                    while(m_pSocket->bytesAvailable()<12){

                        if(!m_pSocket->waitForReadyRead()){
                             m_pSocket->disconnectFromHost();

                        }

                    }
                    xTemp = m_pSocket->read(12);
                    xTemp.clear();
                    //

                     finishByte += file.write(vTemp);//写入数据
                   emit refresh();

                    TotalNum--;
                    //存在一个BUG 尚未进行对12字节处理
                }

                while(m_pSocket->bytesAvailable() < LastBlock){ //对最后一个包进行缓冲

                    if(!m_pSocket->waitForReadyRead()){
                         m_pSocket->disconnectFromHost();
                    }

                }
                vTemp = m_pSocket->read(LastBlock);//读取读最后一块数据

                 finishByte += file.write(vTemp);//写入数据,直到写完才进行下一步
                 emit refresh();

            }
            //数据不足IPMSG_DEFAULT_IOBUFMAX个字节
            else{
                while(m_pSocket->bytesAvailable() < LastBlock){//把数据缓冲下来

                    if(!m_pSocket->waitForReadyRead()){

                         m_pSocket->disconnectFromHost();
                    }

                }
                vTemp = m_pSocket->read(LastBlock);//读取读最后一块数据
                 finishByte += file.write(vTemp);//写入数据,直到写完才进行下一步
               emit refresh();//更新进度

            }

            file.close();//完成一个文件的读写
            totalFileNum--;
        }


         logFile->close();//关闭日志文件
        qDebug()<<"OK";
        finishFlag = true;
        m_pSocket->close();//关闭套接字

}



//失去连接时
void Client::lostConnection()
{
   if(finishFlag) exit(0);//发送完毕，退出程序
    else{
      //  qDebug()<<"Lost connection!";

        std::cout<<'\n'<<"Reconnecting......"<<std::endl;
        connectToServer();//重新连接
        std::cout<<"The network connection has been restored!"<<std::endl;
   }
}




//显示传输进度
void Client::showSpeed()
{
    //326.0 /1571.0MB  16.4MB/S (37%) //输出格式
    printf("%.1f%s%.1f%s%.1lf%s%.2lf%%%s\r",
           (float)finishByte/1048576,
           "/",
           (float)FileLength/1048576,"MB ",
           ((double)finishByte / 1048576)/((double)time.elapsed()/1000),
           "MB/S (",
           100 * (float)finishByte / FileLength ,
           ")");
}




//文件全路径，输出:在该路径上创建文件
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
void Client::connectToServer()
{
    while(true){
        m_pSocket->connectToHost(QHostAddress(ipAddr.c_str()),port);//发起连接
        if(m_pSocket->waitForConnected()) break;//如果连上了服务器，函数返回
    }
}


//找BUG
void Client::test()
{
    /// QByteArray availableByte =  m_pSocket->readAll();//把数据全部读出来
    // qDebug()<<availableByte.length();//显示数据的长度
    qDebug()<<m_pSocket->bytesAvailable();//显示有多少个数据可读

}







