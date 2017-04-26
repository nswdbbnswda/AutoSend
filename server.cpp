#include "server.h"
#include<QHostAddress>
#include<iostream>
#include<QFile>
#include<vector>

Server::Server()
{
   //路径初始化
    std::cout<<"path:";
    std::cin>>dirpath;
    Fileque = new std::queue<QString>;//实例化文件队列
    Pathque = new std::queue<QString>;//实例化路径队列
     FileNum = new char[8];

   //TCP
   server = new QTcpServer(this);//创建TCP套接字
    if (!server->isListening()){//监听
        if (server->listen(QHostAddress::Any, 5555)){
            std::cout<<"open listen port success!"<<std::endl;
        }
        else{ std::cout<<"open listen port fail!"<<std::endl;}
    }
    else{std::cout<<"Function  isListening()  error!"<<std::endl; }
    connect(server, SIGNAL(newConnection()), this, SLOT(newConnectionSlot()));//当有新的连接的时候，就会执行槽函数      
    //connect(&myWatcher,SIGNAL(directoryChanged(QString)),this,SLOT(showMessage(QString)));
   // connect(&myWatcher,SIGNAL(fileChanged(QString)),this,SLOT(showMessage(QString)));
    QString fuck(QString::fromStdString(dirpath));
   // WatchEvery(fuck);//监视当前状态下的所有文件和路径
  //  GetFileList(QString::fromStdString(dirpath));//获得所有的文件存到map中




}

//监视一个目录下的所有文件和所有的目录
void Server::WatchEvery(const QString &path)
{
    myWatcher.addPath(path);
    //定义迭代器并设置过滤器
    QDirIterator dir_iterator1(path,  QDir::NoDotAndDotDot | QDir::AllEntries,QDirIterator::Subdirectories);
    while(dir_iterator1.hasNext())//遍历目录
    {
       QString fuck = dir_iterator1.next();//监视所有目录和文件
       myWatcher.addPath(fuck);
    }
    return;
}




Server::~Server()
{
    if(server) {
        delete server;
    server = NULL;
    }
    if(m_Socket){
        delete m_Socket;
        m_Socket = NULL;
    }

    if(Fileque){
        delete Fileque;
        Fileque = NULL;
    }
    if(FileNum) {
        delete FileNum;
    FileNum = NULL;
    }
    if(Pathque){
       delete Pathque;
    }

}



//遍历目录下的所有文件，把文件存到map里面去
void Server::GetFileList(const QString &path)
{
    QString fuck = path;
    QDir dir(fuck);//实例化一个目录对象
    if(!dir.exists()) //判断路径是否存在
     {
         return ;
     }
     QStringList filters; //获取所选文件类型过滤器
     filters<<QString("*.*");
    // filters<<QString("*.rar")<<QString("*.mp3")<<QString("*.txt")<<QString("*.tiff")<<QString("*.gif")<<QString("*.bmp");
     QDirIterator dir_iterator(fuck,filters,QDir::Files | QDir::NoSymLinks, QDirIterator::Subdirectories);
     while(dir_iterator.hasNext())//遍历目录
     {
        QString temp = dir_iterator.next();
        ++mymapLast[temp];
        Fileque->push(temp);
       // qDebug()<<temp;

//        dir_iterator.next();
//               QFileInfo file_info =dir_iterator.fileInfo();//获得文件列表
//               QString absolute_file_path =file_info.absoluteFilePath();//将文件的绝对路径存到QString
//                Fileque->push(absolute_file_path);//将所有的文件的绝对路径存到队列中
              // string_list.append(absolute_file_path);//(file_path);
              // qDebug()<<absolute_file_path;//打印所有文件的绝对路径





     }
    // qDebug()<<Fileque->size();
}



//当有新的连接的时候
void Server::newConnectionSlot()
{
    //高度抽象为这个新的进行文件夹的同步
    //

    std::cout<<"new connection"<<std::endl;//提示有新的连接
    GetFileList(QString::fromStdString(dirpath));//获得文件列表
    m_Socket = server->nextPendingConnection();//创建连接的套接字
    qint64 i64FileNum =(qint64)Fileque->size();//显示队列中有几个文件
    qDebug()<<i64FileNum;
    //发送文件个数
    memcpy(FileNum, &i64FileNum, 8); //将字节长度信息存在前8个字节内
    m_Socket->write(FileNum,8);//发送文件个数信息




    //发送文件
    while(Fileque->size())//有几个文件就发几次
    {
   //路径获取
    QString fullpath =  Fileque->front();//队列头文件的全路径
    path =  fullpath.toStdString();//把全路径从QString格式转换成string
    //文件打开
    QFile file(path.c_str());
    file.open(QFile::ReadOnly);//以只读的方式打开文件
    std::cout<<path.c_str()<<std::endl;//显示文件的绝对路径
    FileLength = file.size();//获取文件长度
    BlockNum=FileLength/8388608;//整包数量
    temp=BlockNum;//备份整包数量
    LastBlock=FileLength%8388608;//最后一块文件的大小
    TotalNum=BlockNum+1;//总包数量
    TotalByte=BlockNum*8388608+LastBlock;//总字节数
    SendBuffer=new char[8388608+12];//申请buffer
    PathLength=path.length();//储存文件路径字符的长度
    SendPath=new char[PathLength+4];//分配发送路径的buffer,多分配四个字节储存一个整型数
    //拷贝文件名
    memcpy(SendPath, &PathLength, 4); //将字节长度信息存在前4个字节内
    memcpy(&SendPath[4],path.c_str(),PathLength);//组装文件名信息
    m_Socket->write(SendPath,path.length()+4);//发送文件名
    //发送数据
    for (qint32 i = 1; temp> 0; temp--,i++){
        memcpy(SendBuffer, &i, 4);             //添加4个字节到数组的前四个字节作为当前包的次序数
        memcpy(&SendBuffer[4], &TotalNum, 4);// 添加4个字节到数据的4-8字节作为整个文件的包的数量
        memcpy(&SendBuffer[8], &LastBlock, 4); //添加4个字节到数据的8-12字节作为最后一个包的大小
        file.read(&SendBuffer[12],8388608);//读取数据
        lenth = m_Socket->write(SendBuffer,8388608+12);
        m_Socket->waitForBytesWritten();//等待数据发送完
       // emit SendData();
    }
    memcpy(SendBuffer, &TotalNum, 4);//最后一块次序数
    memcpy(&SendBuffer[4], &TotalNum, 4);//总包数
    memcpy(&SendBuffer[8], &LastBlock, 4);
    file.read(&SendBuffer[12],LastBlock);
    m_Socket->write(SendBuffer,LastBlock+12);//发送数据
    m_Socket->waitForBytesWritten();//等待数据发完


    file.close();
    delete []SendBuffer;
    delete []SendPath;
    SendBuffer = NULL;
    SendPath = NULL;
    Fileque->pop();//出队

   }
      std::cout<<"OK"<<std::endl;


}


//显示传输的进度
void  Server::UpProgress(){
    std::cout<<">";
}



//产生要发送的文件队列
void Server::showMessage(const QString &path)
{
    //每发生一次变化都要重新监视所有的文件和文件夹
    // GetFileList(QString("G:/lichao"));//更新监视对象//监视所有文件以及有文件的文件夹
    WatchEvery(QString::fromStdString(dirpath));
    QFileInfo fi(path);
    if(!fi.isFile()){//如果是目录发生了变化，如添加了文件删除了文件等
        QString qpath (QString::fromStdString(dirpath));
        QDir dir(qpath);//实例化一个目录对象
        if(!dir.exists()) //判断路径是否存在
        {
            return ;
        }
        QStringList filters; //获取所选文件类型过滤器
        filters<<QString("*.*");
        QDirIterator dir_iterator(qpath,filters,QDir::Files | QDir::NoSymLinks,  QDirIterator::Subdirectories);
        while(dir_iterator.hasNext())//遍历目录,找出所有的文件
        {
            QString fuck = dir_iterator.next();
            ++mymapCur[fuck];//把所有文件存到map中
        }

        //找出比上次map中多出的文件
        for( auto &w : mymapCur){//如果在当前的这个文件在上一次文件快照的时候没有出现过，那么就把该文件找出来，认为这个文件是刚产生的
            if(mymapLast.find(w.first) == mymapLast.end())
                qDebug()<<w.first;
        }
        mymapLast = mymapCur;//把当前的map作为上次的map记录
        return;
    }

    qDebug()<<path;

}






