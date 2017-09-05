#include "sender.h"

#include<QCoreApplication.h>
//传输发送套接字和队列的指针才能进行启动sender
Sender::Sender( QTcpSocket *socket, std::queue<QString> *queue): m_Socket(socket),Fileque(queue)
{
connect(m_Socket,SIGNAL(disconnected()),SLOT(LostConnection()));//连接显示断开连接的信号与槽
connect(this,SIGNAL(refresh(qint64)),this,SLOT(showSpeed(qint64)));//连接显示传输进度的信号与槽
finishFlag = false;
finishByte = 0;
cunrrentFinishByte = 0;
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
    m_Socket->write(FileNum,8);//发送文件名

    while(Fileque->size())//有几个文件就发几次
    {
        //路径获取
        QString fullpath =  Fileque->front();//队列头文件的全路径
        std::string path =  fullpath.toStdString();//把全路径从QString格式转换成string
        //文件打开
        QFile file(path.c_str());
        file.open(QFile::ReadOnly);//以只读的方式打开文件
        qDebug()<<fullpath;//显示当前传输的文件


        finishByte = 0;




        qint64 FileLength  = file.size();//获取文件长度
        // qDebug()<< FileLength;
        qint64 BlockNum = FileLength/8388608;//整包数量
        qint64 temp = BlockNum;//备份整包数量
        qint64 LastBlock = FileLength%8388608;//最后一块文件的大小
        qint64 TotalNum = BlockNum + 1;//总包数量
        qint64 TotalByte = BlockNum * 8388608 + LastBlock;//总字节数


        char *SendBuffer = new char[8388608 + 12];//申请buffer
        qint32 PathLength = path.length();//储存文件路径字符的长度
        char *SendPath = new char[PathLength + 4];//分配发送路径的buffer,多分配四个字节储存一个整型数

        //拷贝文件名
        memcpy(SendPath, &PathLength, 4); //将字节长度信息存在前4个字节内
        memcpy(&SendPath[4],path.c_str(),PathLength);//组装文件名信息



        m_Socket->write(SendPath,path.length() + 4);//发送文件名

        if(!m_Socket->waitForBytesWritten(600000))//等待数据发完  超过10分钟自己不发了，直接返回，网太差
        {
            return;
        }



        //发送数据
        for (qint32 i = 1; temp> 0; temp--,i++){
            memcpy(SendBuffer, &i, 4);             //添加4个字节到数组的前四个字节作为当前包的次序数
            memcpy(&SendBuffer[4], &TotalNum, 4);// 添加4个字节到数据的4-8字节作为整个文件的包的数量
            memcpy(&SendBuffer[8], &LastBlock, 4); //添加4个字节到数据的8-12字节作为最后一个包的大小
            file.read(&SendBuffer[12],8388608);//读取数据

            finishByte += cunrrentFinishByte = m_Socket->write(SendBuffer,8388608 + 12);//返回实际发送过去的字节数
            emit refresh(FileLength);//发送信号
            if(!m_Socket->waitForBytesWritten(600000)){  //等待数据发送完
                return ;
            }
        }
        memcpy(SendBuffer, &TotalNum, 4);//最后一块次序数
        memcpy(&SendBuffer[4], &TotalNum, 4);//总包数
        memcpy(&SendBuffer[8], &LastBlock, 4);
        file.read(&SendBuffer[12],LastBlock);

        finishByte += cunrrentFinishByte = m_Socket->write(SendBuffer,LastBlock + 12);//发送数据
        emit refresh(FileLength);//显示进度

        if(!m_Socket->waitForBytesWritten(600000)){  //等待数据发完 ,如果断开连接就返回
            return;
        }
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
  //exit(0); //QCoreApplication::exit();//退出程序
//掉线了
else{
//qDebug()<<"Lost connection!";
qDebug()<<"Reconnecting......";
}
}

//刷新速度
void Sender::showSpeed(qint64 fileLenth)
{
  /// printf("%d\r",cunrrentFinishByte / time.elapsed() / 1000 );//每秒发送的字节数



   printf("%.2lf%%\r", 100 * (float)finishByte / fileLenth);
  // printf("%.2lf%%\r", finishByte / fileLenth);
  // qDebug()<<100 * (float)finishByte / fileLenth;
    //qDebug()<<(float)finishByte / fileLenth;

}

