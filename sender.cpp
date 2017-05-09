#include "sender.h"
//传输发送套接字和队列的指针才能进行启动sender
Sender::Sender( QTcpSocket *socket, std::queue<QString> *queue): m_Socket(socket),Fileque(queue)
{



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



    while(Fileque->size())//有几个文件就发几次
    {

        //路径获取
        QString fullpath =  Fileque->front();//队列头文件的全路径
        std::string path =  fullpath.toStdString();//把全路径从QString格式转换成string
        //文件打开
        QFile file(path.c_str());
        file.open(QFile::ReadOnly);//以只读的方式打开文件
        qDebug()<<fullpath;
        qint64 FileLength  = file.size();//获取文件长度
        qint64 BlockNum=FileLength/8388608;//整包数量
        qint64 temp=BlockNum;//备份整包数量
        qint64 LastBlock=FileLength%8388608;//最后一块文件的大小
        qint64 TotalNum=BlockNum+1;//总包数量
        qint64 TotalByte=BlockNum*8388608+LastBlock;//总字节数


        char *SendBuffer=new char[8388608+12];//申请buffer
        qint32 PathLength=path.length();//储存文件路径字符的长度
        char *SendPath=new char[PathLength+4];//分配发送路径的buffer,多分配四个字节储存一个整型数

        //拷贝文件名
        memcpy(SendPath, &PathLength, 4); //将字节长度信息存在前4个字节内
        memcpy(&SendPath[4],path.c_str(),PathLength);//组装文件名信息



        m_Socket->write(SendPath,path.length()+4);//发送文件名

        if(!m_Socket->waitForBytesWritten())//等待数据发完
        {
            return;
        }



        //发送数据
        for (qint32 i = 1; temp> 0; temp--,i++){
            memcpy(SendBuffer, &i, 4);             //添加4个字节到数组的前四个字节作为当前包的次序数
            memcpy(&SendBuffer[4], &TotalNum, 4);// 添加4个字节到数据的4-8字节作为整个文件的包的数量
            memcpy(&SendBuffer[8], &LastBlock, 4); //添加4个字节到数据的8-12字节作为最后一个包的大小
            file.read(&SendBuffer[12],8388608);//读取数据

            m_Socket->write(SendBuffer,8388608+12);


            if(!m_Socket->waitForBytesWritten()){  //等待数据发送完
                return ;
            }


        }
        memcpy(SendBuffer, &TotalNum, 4);//最后一块次序数
        memcpy(&SendBuffer[4], &TotalNum, 4);//总包数
        memcpy(&SendBuffer[8], &LastBlock, 4);
        file.read(&SendBuffer[12],LastBlock);




        m_Socket->write(SendBuffer,LastBlock+12);//发送数据

        if(!m_Socket->waitForBytesWritten()){  //等待数据发完 ,如果断开连接就返回
            return;
        }



        file.close();
        delete []SendBuffer;
        delete []SendPath;
        SendBuffer = NULL;
        SendPath = NULL;
        Fileque->pop();//出队
    }



}

