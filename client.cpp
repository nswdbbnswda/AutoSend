#include "client.h"
#include<QHostAddress>
#include<QDebug>
//启动客户端后就开始连接服务端
Client::Client():NameLength(0),ReceiveName(NULL),m_pSocket(NULL),ReceiveFileNum(NULL),num(0),CurrentNum(0),TotalNum(0),
    LastBlock(0),port(0),Flag(0),TotalByte(0),WrittenByte(0),FileNumber(0)
{
    std::cout<<"IP:";
    std::cin>>ipAddr;
   // ipAddr = std::string("127.0.0.1");


    m_pSocket = new QTcpSocket();//创建客户端套接字
    m_pSocket->connectToHost(QHostAddress(ipAddr.c_str()),5555);//发起连接
   //连接信号和槽
    connect(this, SIGNAL(DataComing()),this, SLOT(ReceiveData()));
    emit DataComing();//发送信号，文件来了
   // connect(m_pSocket,SIGNAL(readyRead()),this,SLOT(test()));
}







Client::~Client()
{
    if(m_pSocket) {
        delete m_pSocket;
        m_pSocket = NULL;
    }

}



//接收数据
void Client::ReceiveData()
{

    while(1)//一直等待接收数据
    {
        //while(m_pSocket->waitForReadyRead(50000))//当有数据可读时
        m_pSocket->waitForReadyRead();
        while(m_pSocket->bytesAvailable())//有数据可读的时候才进来
        {
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
            vTemp.insert(0,QByteArray("E"));//更改为E盘符
            //搞定路径
            QString fullPath(vTemp);//QByteArray 转换成 QString
            KoPath(fullPath);//搞定路径问题,如果没有则创建

            qDebug()<<fullPath;
            //在硬盘上建立该文件
            QFile file(vTemp.data());
            file.open(QFile::WriteOnly);//只写方式打开
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
                    vTemp=m_pSocket->read(8388608); //读取838608个字节
                    //vTemp.clear();
                    while(m_pSocket->bytesAvailable()<12){
                        m_pSocket->waitForReadyRead();
                    }
                    xTemp=m_pSocket->read(12);
                    xTemp.clear();
                    WrittenByte+=file.write(vTemp);//写入数据
                    TotalNum--;
                    //存在一个BUG 尚未进行对12字节处理
                }
                while(m_pSocket->bytesAvailable()<LastBlock){
                    m_pSocket->waitForReadyRead();
                }
                vTemp=m_pSocket->read(LastBlock);//第一次读取读最后一块数据
                WrittenByte+=file.write(vTemp);
               // vTemp.clear();
            }

            else{//处理最后一个数据块
                while(m_pSocket->bytesAvailable()<LastBlock){
                    m_pSocket->waitForReadyRead();
                }
                vTemp=m_pSocket->read(LastBlock);//第一次读取读最后一块数据
                WrittenByte+=file.write(vTemp);
               // vTemp.clear();
                //emit  DataWritten();
            }
            file.close();
        }

    }

}





































//打印"进度条"
void  Client::UpProgress(){
    std::cout<<">";
}

void Client::test()
{
   //qDebug()<<m_pSocket->bytesAvailable();
   // m_pSocket->readAll();



//           while(m_pSocket->bytesAvailable()<4){
//            m_pSocket->waitForReadyRead();
//            }
//            vTemp=m_pSocket->read(4);//读4个字节，储存的是文件名字的字节数量
//            ReceiveName=vTemp.data();
//            memcpy(&NameLength,ReceiveName,4);//NameLength储存的是文件名字所占的字节数量
////            while(m_pSocket->bytesAvailable()<NameLength)
////            {
////                  m_pSocket->waitForReadyRead();
////            }
//            vTemp=m_pSocket->read(NameLength);//读文件名字  读NameLength个字节
//            vTemp.remove(0,1);//删除第一个字节（盘符）
//            vTemp.insert(0,QByteArray("E"));//更改为E盘符
//            std::cout<<vTemp.toStdString()<<std::endl;//显示文件全路径名(包括文件名)

//           //搞定路径
//            QString fullPath(vTemp);//QByteArray 转换成 QString
//            KoPath(fullPath);//搞定路径问题,如果没有则创建

//            //在硬盘上建立该文件
//            QFile file(vTemp.data());
//            file.open(QFile::WriteOnly);//只写方式打开
//            //读12个字节的文件头信息
//            vTemp=m_pSocket->read(12);//读12字节
//            ReceiveHead=vTemp.data();//类型转换QByteArray到char *
//            memcpy(&CurrentNum,ReceiveHead,4);
//            memcpy(&TotalNum,&ReceiveHead[4],4);//整个文件的包的数量
//            memcpy(&LastBlock,&ReceiveHead[8],4);//当前文件最后一个包的字节数
//            TotalByte=(TotalNum-1)*8388608+LastBlock;//当前文件的总的字节数量

//            if(TotalNum>1){
//                while(TotalNum-1){
//                    vTemp=m_pSocket->read(8388608); //读取838608个字节
//                    while(vTemp.length()<8388608){//如果缓存中没有那么多数据
//                        m_pSocket->waitForReadyRead();//缓冲
//                        vTemp.append(m_pSocket->read(8388608-vTemp.length()));
//                    }
//                    xTemp=m_pSocket->read(12);
//                    vTemp.clear();//add
//                   // WrittenByte+=file.write(vTemp);//写入数据

//                    TotalNum--;
//                    //存在一个BUG 尚未进行对12字节处理
//                }
//                vTemp=m_pSocket->read(LastBlock);//第一次读取读最后一块数据
//                while(vTemp.length()<LastBlock){//判断是否够最后一块数据长度
//                    m_pSocket->waitForReadyRead();//缓冲
//                    vTemp.append(m_pSocket->read(LastBlock-vTemp.length()));//拼接数组
//                }
//                vTemp.clear();//add
//               // WrittenByte+=file.write(vTemp);

//            }

//            else{//处理最后一个数据块

//                vTemp=m_pSocket->read(LastBlock);//第一次读取读最后一块数据
//                while(vTemp.length()<LastBlock){//判断是否够最后一块数据长度
//                    m_pSocket->waitForReadyRead();//缓冲
//                    vTemp.append(m_pSocket->read(LastBlock-vTemp.length()));//拼接数组
//                }
//                vTemp.clear();
//               // WrittenByte+=file.write(vTemp);
//                //emit  DataWritten();
//            }
//            file.close();


}



//输入:文件全路径，输出:在该路径上创建文件
bool  Client::KoPath(const QString &dirName)//文件全路径(包含文件名）
{
    QString fullPath;
    QFileInfo fileInfo(dirName);
    fullPath = fileInfo.absolutePath();//全路径，不包括文件名
    QDir dir(fullPath);//创建目录对象
    if(dir.exists())
    {
        return true;
    }
    else
    {
        bool ok = dir.mkpath(fullPath);//创建多级目录
        return ok;
    }
}






