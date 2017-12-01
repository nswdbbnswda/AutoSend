#include "client.h"
#include<QHostAddress>
#include<QDebug>
#include <QCoreApplication>
#include"autosend.h"

//启动客户端后就开始连接服务端
Client::Client(const std::string strIpAddr,const std::string  inputPort,const std::string _savePath)
{
    nameLength = 0;
    receiveName = NULL;
    m_pSocket = NULL;
    receiveFileNum = NULL;
    taskType = 0;
    num = 0;
    CurrentNum = 0;
    TotalNum = 0;
    LastBlock = 0;
    Flag = 0;
    FileLength = 0;
    FileNumber = 0;
    ipAddr = strIpAddr;
    savePath = _savePath;
    finishByte = 0;
    cunrrentFinishByte = 0;
    fileStartPos = 0;
    fileNameSeparator = '|';
    finishFlag = false;
    taskType = TaskType::BREAKTASK;//默认为新任务
    port = atoi(inputPort.c_str());//把端口号从string 类型转换成 整型
    m_pSocket = new QTcpSocket();//创建客户端套接字
    m_pSocket->setReadBufferSize(IPMSG_DEFAULT_QTINERBUFFER);//设置QT内部缓冲区大小为10M


    logFile = new QFile(QCoreApplication::applicationDirPath()+ "/log.txt");//以本次任务编号为名字创建日志文件
    logFile->open(QIODevice::ReadWrite | QIODevice::Truncate);//覆盖打开日志


    connectToServer();//连接服务器
    connect(this,SIGNAL(refresh()),this,SLOT(showSpeed()));
    connect(m_pSocket,SIGNAL(disconnected()),this,SLOT(lostConnection()));//断开连接了做异常处理
    //连接信号和槽将数据接收并存到硬盘中
    connect(this, SIGNAL(dataComing()),this, SLOT(receiveData()));
    //emit dataComing();//发送信号，文件来了
    connect(this,SIGNAL(taskCodeComing()),this,SLOT(responseTask()));


     emit taskCodeComing();//接收任务代号

}


Client::~Client()
{
    if(m_pSocket) {delete m_pSocket; m_pSocket = NULL; }
    if(logFile){delete logFile; logFile = NULL; }
}


//接收数据
void Client::receiveData()
{ 
    //等待发送端把文件的个数发送的过来
    QByteArray  fileNum;
    qint64  totalFileNum = 0;
    while(m_pSocket->bytesAvailable()<8){//等待至少有8个字节数据到来
        m_pSocket->waitForReadyRead();
    }
    fileNum = m_pSocket->read(8);//把这8个字节读到字节数组里
    receiveFileNum = fileNum.data();//转换成char *类型
    memcpy(&totalFileNum,receiveFileNum,8);//拷贝到totalFileNum变量中
     qDebug()<<"File number:"<<totalFileNum;//显示文件个数

    //循环接收每一个文件
    while(totalFileNum){
        time.restart();
        finishByte = 0;

        while(m_pSocket->bytesAvailable()<4){//保证至少先读到储存文件名长度的变量
            m_pSocket->waitForReadyRead();
        }
        vTemp = m_pSocket->read(4);//读4个字节，储存的是文件名字的字节数量
        receiveName = vTemp.data();//转换成char*类型
        memcpy(&nameLength,receiveName,4);//nameLength储存的是文件名字所占的字节数量
        while(m_pSocket->bytesAvailable() < nameLength){//如果当前缓冲区字节数不足nameLength，就缓冲到足为止
            m_pSocket->waitForReadyRead();
        }

        vTemp = m_pSocket->read(nameLength);//读文件名字  读NameLength个字节


        QString  fullPath = vTemp;


        makePath(fullPath);//搞定路径问题,如果没有则创建

        //qDebug()<<fullPath;

         qDebug()<<fullNameT;


        // QString fullPath(vTemp);//QByteArray 转换成 QString
        //qDebug()<<fullNameT;//显示当前传输的文件名称
        //在硬盘上建立该文件
       // vTemp = fullNameT.toLatin1();
       // qDebug()<<vTemp.data();



        QFile file(fullNameT);


        file.open(QIODevice::ReadWrite|QIODevice::Truncate);//删除文件内容再进行写入
        logFile->write(fullNameT.toStdString().c_str()); //写入日志文件

        logFile->flush();//刷新一下内容
        logFile->write("\r\n");//日志格式


        //读12个字节的文件头信息
        while(m_pSocket->bytesAvailable() < 12){
            m_pSocket->waitForReadyRead();
        }
        vTemp = m_pSocket->read(12);//读12字节
        ReceiveHead = vTemp.data();//类型转换QByteArray到char *
        memcpy(&CurrentNum,ReceiveHead,4);
        memcpy(&TotalNum,&ReceiveHead[4],4);//整个文件的包的数量
        memcpy(&LastBlock,&ReceiveHead[8],4);//当前文件最后一个包的字节数

        FileLength = (qint64)((qint64)(TotalNum-1) * (qint64)IPMSG_DEFAULT_IOBUFMAX +(qint64) LastBlock);//当前文件的总的字节数量

        if(TotalNum>1){//如果存在整包
            while(TotalNum-1){//运行的次数
                while(m_pSocket->bytesAvailable() < IPMSG_DEFAULT_IOBUFMAX){//缓冲IPMSG_DEFAULT_IOBUFMAX个字节数
                    m_pSocket->waitForReadyRead();
                }
                vTemp = m_pSocket->read(IPMSG_DEFAULT_IOBUFMAX); //读取IPMSG_DEFAULT_IOBUFMAX个字节
                while(m_pSocket->bytesAvailable() < 12){
                    m_pSocket->waitForReadyRead();
                }
                xTemp = m_pSocket->read(12);
                xTemp.clear();
                finishByte += file.write(vTemp);//写入数据
                emit refresh();
                TotalNum--;
                //存在一个BUG 尚未进行对12字节处理
            }
            while(m_pSocket->bytesAvailable() < LastBlock){ //对最后一个包进行缓冲
                m_pSocket->waitForReadyRead();
            }
            vTemp = m_pSocket->read(LastBlock);//读取读最后一块数据
            finishByte += file.write(vTemp);//写入数据,直到写完才进行下一步
            emit refresh();
        }
        //数据不足IPMSG_DEFAULT_IOBUFMAX个字节
        else{
            while(m_pSocket->bytesAvailable() < LastBlock){//把数据缓冲下来
                m_pSocket->waitForReadyRead();
            }
            vTemp = m_pSocket->read(LastBlock);//读取读最后一块数据
            finishByte += file.write(vTemp);//写入数据,直到写完才进行下一步
            emit refresh();//更新进度
        }
        file.close();//完成一个文件的读写
        totalFileNum--;
    }
    qDebug()<<"Receive OK";
    finishFlag = true;
    m_pSocket->close();
}


//失去连接时
void Client::lostConnection()
{
    if(finishFlag) {//发送成功退出的时候清除日志
        logFile->close();//关闭日志文件
        exit(0);//发送完毕，退出程序
    }
    else{
        logFile->close();//关闭日志文件
        exit(0);
    }
}


//显示传输进度
void Client::showSpeed()
{
    //326.0 /1571.0MB  16.4MB/S (37%) //输出格式
    printf("%.1lf%s%.1lf%s%.1lf%s%.2lf%%%s\r",
           (double)finishByte/1048576,
           "/",
           (double)FileLength/1048576,"MB ",
           ((double)finishByte / 1048576)/((double)time.elapsed()/1000),
           "MB/S (",
           100 * (double)finishByte / FileLength ,
           ")");
}



//文件全路径，输出:在该路径上创建文件
bool  Client::makePath(const QString &dirName)//文件全路径(包含文件名）
{
    QString prePath = QString::fromStdString(savePath);//自定义的路径
    QFileInfo fileInfo(dirName);
    fullNameT = prePath + "/" + fileInfo.fileName();//要储存的全路径


      QDir dir(prePath);//创建目录对象
      if(dir.exists()){   return true; }
      else{
          bool ok = dir.mkpath(prePath);//创建多级目录
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


//从日志中找到断点
bool Client::findBreakPoint(const QByteArray & logArray)
{
    qint64 breakFileTailSymbolPos;//断点文件尾部符号'|'位置
    qint64 breakPointFileHeadPos;//断点文件头部'|'符号位置
    if(logArray.isEmpty()){
        return false;//日志内容为空
    }
    //找到最后一个文件的左侧 '|' 符号位置  例如: |D:/1.txt|
    breakFileTailSymbolPos = fileStartPos - 1;
    breakPointFileHeadPos = logArray.lastIndexOf('|',breakFileTailSymbolPos - 1); //找到断点文件头部符号位置
    breakFileName = logArray.mid(breakPointFileHeadPos + 1,breakFileTailSymbolPos - breakPointFileHeadPos -1); //截取断点文件名字
    qDebug()<<breakFileName;
    logFile->seek(breakPointFileHeadPos + 1);//调整文件指针位置，到断点文件的起始位置
    return true;
}


//响应任务
void Client::responseTask()
{
    if(!makePath(QCoreApplication::applicationDirPath()+ "/record" + "/recordfile")){//创建record目录
        qDebug()<<"make logfile path false!";//创建目录失败
        return;
    }
    m_pSocket->waitForReadyRead();//阻塞等待至少有一个字节可读
    taskCode = m_pSocket->readAll();//接收任务编号
    QString taskCodeFile = taskCode;

    sendIndexPos();//请求服务端发送文件
    emit dataComing();//发送信号，接收文件
}


//向服务端请求从索引处发送文件
bool  Client::sendIndexPos(const QString & name,qint64 pos)
{
    QByteArray requestContext;
    requestContext.append(name);
    requestContext.append('|');
    requestContext.append(QString::number(pos,10));// "|0"表示 告诉服务器从头开始发
    m_pSocket->write(requestContext);//发送请求
    m_pSocket->waitForBytesWritten();
    return true;
}












