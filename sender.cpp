
#include "sender.h"
#include<windows.h>
#include<QCoreApplication.h>
#include"autosend.h"
#include "filewatcher.h"
#include "server.h"
#include"Factoary.h"




//传输发送套接字和队列的指针才能进行启动sender
Sender::Sender()
{
    m_Socket = NULL;
    connect(this,SIGNAL(refresh(qint64)),this,SLOT(showSpeed()));//连接显示传输进度的信号与槽
    finishFlag = false;
    finishByte = 0;
    cunrrentFinishByte = 0;
    FileLength = 0;
    breakFlag = false;

    pathQueue = Factoary::pathlist;

    FileWatcher::getInstance()->GetFileList(pathQueue,nametoFullPath,fileQueue,fullPathtoName);//通过路径列表获得文件队列

}

Sender::~Sender()
{
    m_Socket->close();
    if(m_Socket){ delete m_Socket; }
}


//发送文件
void Sender::sendFile(std::queue<QString> &curfileQueue ,qint64 pos)
{


    breakFlag = pos;
    qint64 i64FileNum =(qint64)curfileQueue.size();//显示队列中有几个文件
    //qDebug()<<i64FileNum;
    //告诉对面有多少个文件要接收
    char *FileNum = new char[i64FileNum];//文件个数数组
    memcpy(FileNum, &i64FileNum, 8); //将字节长度信息存在前4个字节内
    if( -1 == m_Socket->write(FileNum,8)){//把文件个数发过去
        qDebug()<<"发送数据失败m_Socket->write() line 46";
        return;
    }
    time.start();//开始计时
    while(curfileQueue.size())//有几个文件就执行几次
    {

        //路径获取
        QString fullpath =  curfileQueue.front();//文件队列中第一个文件的全路径
        std::string path =  fullpath.toStdString();//把全路径从QString格式转换成string
        QFile file(path.c_str());    //在硬盘上打开这个文件

        if(!file.open(QFile::ReadOnly)){qDebug()<<"打开文件"<<fullpath<<"失败";}

        //qDebug()<<fullpath;//显示当前传输的文件

        //此处需要发送给接收端整理好的文件名（文件夹就带路径名，文件就是只有文件名字）
        path = fullPathtoName[fullpath].toStdString();//将文件队列中的全路径转换成发送格式

        FileLength  = file.size();//获取文件长度
        if(breakFlag){//断点续传任务
            FileLength = FileLength - pos;//将文件剩余的部分 进行发送
            file.seek(pos);//移动文件指针到断点处
            breakFlag = false;
        }

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
        if( -1 == m_Socket->write(SendPath,path.length() + 4)){//发送文件名
            qDebug()<<"m_Socket->write(SendPath,path.length() + 4)) line 87";
            return;
        }
        if(!m_Socket->waitForBytesWritten(IPMSG_DEFAULT_WAITTIMEMAX))//等待数据发完  超过10分钟自己不发了，直接返回，网太差
        {
         //   qDebug()<<"waitForBytesWritten() line 91";
            return;
        }
        //发送数据
        for (qint32 i = 1; temp > 0; temp--,i++){
            memcpy(SendBuffer, &i, 4);             //添加4个字节到数组的前四个字节作为当前包的次序数
            memcpy(&SendBuffer[4], &TotalNum, 4);// 添加4个字节到数据的4-8字节作为整个文件的包的数量
            memcpy(&SendBuffer[8], &LastBlock, 4); //添加4个字节到数据的8-12字节作为最后一个包的大小

            file.read(&SendBuffer[12],IPMSG_DEFAULT_IOBUFMAX);//读取数据

            // finishByte += cunrrentFinishByte = m_Socket->write(SendBuffer,IPMSG_DEFAULT_IOBUFMAX + 12);//发送
            if(-1 != (cunrrentFinishByte = m_Socket->write(SendBuffer,IPMSG_DEFAULT_IOBUFMAX + 12))){//发送

                finishByte += (cunrrentFinishByte - 12);
            }
            else{
                qDebug()<<"m_Socket->write(SendBuffer,IPMSG_DEFAULT_IOBUFMAX + 12) line 107";
                return;
            }
            if(!m_Socket->waitForBytesWritten(IPMSG_DEFAULT_WAITTIMEMAX)){  //等待数据发送完
              //  qDebug()<<"waitForBytesWritten() line 110";
                return ;
            }
            emit refresh(cunrrentFinishByte);//发送信号

        }
        memcpy(SendBuffer, &TotalNum, 4);//最后一块次序数
        memcpy(&SendBuffer[8], &LastBlock, 4);
        memcpy(&SendBuffer[4], &TotalNum, 4);//总包数
        file.read(&SendBuffer[12],LastBlock);
        //finishByte += cunrrentFinishByte = m_Socket->write(SendBuffer,LastBlock + 12);//发送数据
        if(-1 != (cunrrentFinishByte = m_Socket->write(SendBuffer,LastBlock + 12))){
            finishByte += (cunrrentFinishByte - 12);
        }
        else{
            qDebug()<<"m_Socket->write(SendBuffer,LastBlock + 12) line 125";
            return ;
        }
        if(!m_Socket->waitForBytesWritten(IPMSG_DEFAULT_WAITTIMEMAX)){  //等待数据发完 ,如果断开连接就返回
          //  qDebug()<<"waitForBytesWritten() line 128";
            return;
        }
        emit refresh(cunrrentFinishByte);//显示进度
        file.close();
        delete []SendBuffer;
        delete []SendPath;
        SendBuffer = NULL;
        SendPath = NULL;
        curfileQueue.pop();//出队
    }

    delete FileNum;
    //qDebug()<<"发送完成!";
    finishFlag = true;

    //m_Socket->disconnectFromHost();//当所有的数据从发送端主机发送到网络上以后，会主动触发一个disconnected信号
}




//发送任务代号
void Sender::sendTaskCode()
{   
    QString s = QString::number(nameHash(fileQueue),10);//获得由所有名字计算求得的hash值
    //把总文件大小也作为任务代号的一部分
    s += "+";
    s += QString::number(FileWatcher::getInstance()->GetTotalFileSize(),10);

    QByteArray data = s.toLatin1();//QString 类型转换成 QByteArray类型
    m_Socket->write(data);
    m_Socket->waitForBytesWritten();
}


//设置发送套接字
void Sender::setSocket(QTcpSocket *socket)
{
    m_Socket = socket;//更新套接字
    connect(m_Socket,SIGNAL(disconnected()),this,SLOT(LostConnection()));//重新连接
    connect(m_Socket,SIGNAL(readyRead()),this,SLOT(acceptRequest()));
}



//调整文件队列到断点前状态
bool Sender::adjustedQueues(const QByteArray &fileName , std::queue<QString> &fileQue)
{
    QString qStringFileName = fileName;
    while(fileQue.size()){
        if(qStringFileName != fileQue.front()){
            fileQue.pop();
        }else{ break; }
    }
    return fileQue.size() > 0;
}



//制作任务编号
unsigned long Sender::nameHash( std::queue<QString> fileNameQue)
{
    //用第一个元素进行初始化
    size_t str_hash;//定义储存代号值的变量
    std::queue<QString> tempQueue = fileNameQue;//拷贝一次文件名队列
    std::string str = tempQueue.front().toStdString();//获得队头元素值
    std::hash<std::string> hash_fn;//哈希算法
    str_hash = hash_fn(str);//初始化代号值
    tempQueue.pop();
    //将剩下的部分全部进行与运算
    while(!tempQueue.empty()){//将每一个文件名字按位与运算

        str = tempQueue.front().toStdString();//取队首元素
        str_hash = str_hash >> 2;//左移两位
        str_hash |= hash_fn(str);//求的hash值并且进行按位与运算
        tempQueue.pop();//队头元素出队
    }
    return str_hash;
}


//断线处理
void Sender::LostConnection()
{
  //  if( finishFlag) //发完了
     //   emit finishSend();//发个信号通知主程序退出

   // else{//掉线了
        std::cout<<'\n'<<"Reconnecting......"<<std::endl;
        disconnect(m_Socket,SIGNAL(disconnected()),this,SLOT(LostConnection()));//解开连接
        disconnect(m_Socket,SIGNAL(readyRead()),this,SLOT(acceptRequest()));//解开连接
  //  }
}


//显示进度速度等信息
void Sender::showSpeed()
{
    //326.0 /1571.0MB  16.4MB/S (37%) //输出格式
    printf("%.1lf%s%.1lf%s%.1lf%s%.2lf%%%s\r",
           (double)finishByte / 1048576,
           "/",
           (double)FileWatcher::getInstance()->GetTotalFileSize() / 1048576,"MB ",
           ((double)finishByte / 1048576)/((double)time.elapsed()/1000),//平均速度
           "MB/S (",
           100 * (double)finishByte / FileWatcher::getInstance()->GetTotalFileSize() ,
           ")");
}


//接收请求并处理
void Sender::acceptRequest()
{
    QByteArray  messageContext;
    messageContext = m_Socket->read(1);//先读一个字符如果是":"说明是退出指令
    if(":" == messageContext){ emit finishSend();}
    else if("?" == messageContext){//这是客户端的心跳
        return;//接收就好，不做任何处理
    }
    else messageContext.append(m_Socket->readAll());

    int indexPos = messageContext.indexOf('|');//找到第一个出现'|'的索引位置
    if( -1 == indexPos) return;

    QByteArray fileName = messageContext.left(indexPos);//获取文件名
    QByteArray filePos = messageContext.right(messageContext.length() - indexPos -1);//获取断点位置

    if(fileName.length() == 0 && filePos == "0"){
        std::queue<QString>fileQueCur = fileQueue;
        sendFile(fileQueCur);//发送全部文件
        //

    }
    else{//断点任务

        QString hardPath = nametoFullPath[QString(fileName)];//名字转换成路径
        fileName = hardPath.toUtf8();//转换回QByteArray类型的fileName
        std::queue<QString> sendQue = fileQueue;//获取一个完整文件队列
        if(adjustedQueues(fileName,sendQue)){//把队列待发送队列调整到断点前状态
            QString intPos = filePos;
            qint64 qintPos = intPos.toInt();//转换成整型

            qDebug()<<"断点位置:"<<fileName<<intPos;
            sendFile(sendQue,qintPos);//发送文件   
        }
        else{//如果在队列中没有找到这个文件
            qDebug()<<"没有在发送队列中找到该断点文件!";
            return;
        }
    }
}




