#include "sender.h"
#include<windows.h>
#include<QCoreApplication.h>
#include"autosend.h"
//传输发送套接字和队列的指针才能进行启动sender
Sender::Sender( QTcpSocket *socket, std::queue<QString> *queue): m_Socket(socket),Fileque(queue)
{
connect(m_Socket,SIGNAL(disconnected()),SLOT(LostConnection()));//连接显示断开连接的信号与槽
connect(this,SIGNAL(refresh(qint64)),this,SLOT(showSpeed(qint64)));//连接显示传输进度的信号与槽
finishFlag = false;
finishByte = 0;
cunrrentFinishByte = 0;
FileLength = 0;

}

Sender::~Sender()
{
if(m_Socket){
    delete m_Socket;
}
if(Fileque){
    delete Fileque;
}
}


//发送文件
void Sender::sendFile()
{
    qint64 i64FileNum =(qint64)Fileque->size();//显示队列中有几个文件
    //qDebug()<<i64FileNum;

    //告诉对面有多少个文件要接收
    char *FileNum = new char[i64FileNum];//文件个数数组
    memcpy(FileNum, &i64FileNum, 8); //将字节长度信息存在前4个字节内
    m_Socket->write(FileNum,8);//把文件个数发过去


    while(Fileque->size())//有几个文件就执行几次
    {
         time.restart();
        //路径获取
        QString fullpath =  Fileque->front();//文件队列中第一个文件的全路径
        std::string path =  fullpath.toStdString();//把全路径从QString格式转换成string

        QFile file(path.c_str());    //在硬盘上打开这个文件
        file.open(QFile::ReadOnly);//以只读的方式打开
        qDebug()<<fullpath;//显示当前传输的文件
        finishByte = 0;
        FileLength  = file.size();//获取文件长度
        // qDebug()<< FileLength;
        qint64 BlockNum = FileLength / IPMSG_DEFAULT_IOBUFMAX;//整包数量
        qint64 temp = BlockNum;//备份整包数量
        qint64 LastBlock = FileLength % IPMSG_DEFAULT_IOBUFMAX;//最后一块文件的大小
        qint64 TotalNum = BlockNum + 1;//总包数量
        qint64 TotalByte = BlockNum * IPMSG_DEFAULT_IOBUFMAX + LastBlock;//总字节数


        char *SendBuffer = new char[IPMSG_DEFAULT_IOBUFMAX + 12];//申请buffer
        qint32 PathLength = path.length();//计算储存文件路径字符串的长度
        char *SendPath = new char[PathLength + 4];//分配发送路径的buffer,多分配四个字节储存一个整型数

        //拷贝文件名
        memcpy(SendPath, &PathLength, 4); //将字节长度信息存在前4个字节内
        memcpy(&SendPath[4],path.c_str(),PathLength);//组装文件名信息
        m_Socket->write(SendPath,path.length() + 4);//发送文件名
        if(!m_Socket->waitForBytesWritten(IPMSG_DEFAULT_WAITTIMEMAX))//等待数据发完  超过10分钟自己不发了，直接返回，网太差
        {
            return;
        }

        //发送数据
        for (qint32 i = 1; temp > 0; temp--,i++){
            memcpy(SendBuffer, &i, 4);             //添加4个字节到数组的前四个字节作为当前包的次序数
            memcpy(&SendBuffer[4], &TotalNum, 4);// 添加4个字节到数据的4-8字节作为整个文件的包的数量
            memcpy(&SendBuffer[8], &LastBlock, 4); //添加4个字节到数据的8-12字节作为最后一个包的大小

            file.read(&SendBuffer[12],IPMSG_DEFAULT_IOBUFMAX);//读取数据

            finishByte += cunrrentFinishByte = m_Socket->write(SendBuffer,IPMSG_DEFAULT_IOBUFMAX + 12);//发送

            if(!m_Socket->waitForBytesWritten(IPMSG_DEFAULT_WAITTIMEMAX)){  //等待数据发送完
                return ;
            }
            emit refresh(cunrrentFinishByte);//发送信号

        }
        memcpy(SendBuffer, &TotalNum, 4);//最后一块次序数
        memcpy(&SendBuffer[4], &TotalNum, 4);//总包数
        memcpy(&SendBuffer[8], &LastBlock, 4);
        file.read(&SendBuffer[12],LastBlock);

        finishByte += cunrrentFinishByte = m_Socket->write(SendBuffer,LastBlock + 12);//发送数据



        if(!m_Socket->waitForBytesWritten(IPMSG_DEFAULT_WAITTIMEMAX)){  //等待数据发完 ,如果断开连接就返回
            return;
        }
        emit refresh(cunrrentFinishByte);//显示进度

        file.close();
        delete []SendBuffer;
        delete []SendPath;
        SendBuffer = NULL;
        SendPath = NULL;
        Fileque->pop();//出队
    }
    delete FileNum;

    qDebug()<<"OK";
    finishFlag = true;
}








void Sender::LostConnection(){ 

if( finishFlag) //发完了
    emit finishSend();//发个信号通知主程序退出
//掉线了
else{

std::cout<<'\n'<<"Reconnecting......";

 }
}

//刷新速度
void Sender::showSpeed(qint64 finishSend)
{


  //326.0 /1571.0MB  16.4MB/S (37%) //输出格式
   //printf("%10d%.2lf%%%5d\r",finishByte,100 * (float)finishByte / fileLenth,time.elapsed() / 1000);
   printf("%.1f%s%.1f%s%.1lf%s%.2lf%%%s\r",
   (float)finishByte/1048576,
          "/",
          (float)FileLength/1048576,"MB ",
          ((double)finishByte / 1048576)/((double)time.elapsed()/1000),
         "MB/S (",
         100 * (float)finishByte / FileLength ,
          ")");

   // qDebug()<<cunrrentFinishByte;//显示本次发送一共发送了多少个字节


  // printf("%.2lf%%\r", finishByte / fileLenth);
  // qDebug()<<100 * (float)finishByte / fileLenth;
    //qDebug()<<(float)finishByte / fileLenth;


}

