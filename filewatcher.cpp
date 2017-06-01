#include "filewatcher.h"
#include<iostream>




std::queue<QString> FileWatcher::fileQueue;//定义一个静态队列



FileWatcher::FileWatcher(const std::string &s):FileBase(s)
{
    connect(&myWatcher,SIGNAL(directoryChanged(QString)),this,SLOT(findChangefile(QString)));
    connect(&myWatcher,SIGNAL(fileChanged(QString)),this,SLOT(findChangefile(QString)));
    GetFileList((QString::fromStdString(strPath)),mymapLast);//遍历一边目录找出所有文件存到Map中作为map第一次记录
    qDebug()<<"File number:"<<mymapLast.size();//显示文件个数


}

FileWatcher::~FileWatcher()
{

}


//遍历目录下的所有文件，把文件存到map里面去
void FileWatcher::GetFileList(const QString &path, std::map<QString,size_t> &saveMap)
{
    QString fuck = path;
    QDir dir(fuck);//实例化一个目录对象
    if(!dir.exists()) //判断路径是否存在
     {
         return ;
     }
    // QStringList filters; //获取所选文件类型过滤器
    /// filters<<QString("*.*");//
    // QDirIterator dir_iterator(fuck,filters,QDir::Files | QDir::NoSymLinks|, QDirIterator::Subdirectories);
  //   QDirIterator dir_iterator(fuck,filters,QDir::Files | QDir::NoSymLinks|QDir::Hidden, QDirIterator::Subdirectories);//能筛出隐藏文件,但是不能筛出不带后缀名的文件
      //QDirIterator dir_iterator(fuck,filters,QDir::Files | QDir::Hidden | QDir::NoSymLinks);
      QDirIterator dir_iterator(fuck,QDir::Files|QDir::Hidden,QDirIterator::Subdirectories);//能筛出所有文件,不能遍历出快捷方式QDir::NoSymLinks
      //QDirIterator dir_iterator(fuck,QDir::Files |QDir::Hidden);//能筛出所有文件

     while(dir_iterator.hasNext())//遍历目录
     {
       QString temp = dir_iterator.next();
      ++saveMap[temp];//把遍历出来的文件存到map中
     }



}




//根据给定的路径遍历文件夹,并且把遍历到的文件列表存到队列中
void FileWatcher::GetFileList(const QString &path, std::queue<QString> &saveQueue)
{
    QString fuck = path;
    QDir dir(fuck);//实例化一个目录对象
    if(!dir.exists()) //判断路径是否存在
     {
         return ;
     }
     QStringList filters; //获取所选文件类型过滤器
     filters<<QString("*.*");
    // QDirIterator dir_iterator(fuck,filters,QDir::Files | QDir::NoSymLinks, QDirIterator::Subdirectories);
     QDirIterator dir_iterator(fuck,QDir::Files | QDir::NoSymLinks|QDir::Hidden, QDirIterator::Subdirectories);//能筛出所有文件

     while(dir_iterator.hasNext())//遍历目录
     {
        QString temp = dir_iterator.next();
       saveQueue.push(temp);//把遍历出来的文件都存储到队列中
     }
}




//监视一个目录下的所有文件和所有的目录
void  FileWatcher::watchEverything(){

    myWatcher.addPath(QString::fromStdString(strPath));
    //定义迭代器并设置过滤器
    QDirIterator dir_iterator1(QString::fromStdString(strPath),  QDir::NoDotAndDotDot | QDir::AllEntries,QDirIterator::Subdirectories);
    while(dir_iterator1.hasNext())//遍历目录
    {
        QString fuck = dir_iterator1.next();//监视所有目录和文件
        myWatcher.addPath(fuck);
    }
    return;
}




//找到变化的文件,存到队列中，然后把这个队列发射出去
void FileWatcher::findChangefile(const QString &path){

     watchEverything(); //每发生一次变化都要重新监视所有的文件和文件夹
     QFileInfo fi(path);
    if(!fi.isFile()){//如果是目录发生了变化，如添加了文件删除了文件等
       QString qpath (QString::fromStdString(strPath));
       QDir dir(qpath);//实例化一个目录对象
        if(!dir.exists()) //判断路径是否存在
        {
           return ;
        }
      QStringList filters; //获取所选文件类型过滤器
      filters<<QString("*.*");
      // QDirIterator dir_iterator(qpath,filters,QDir::Files | QDir::NoSymLinks,  QDirIterator::Subdirectories);
       QDirIterator dir_iterator(qpath,QDir::Files | QDir::NoSymLinks|QDir::Hidden, QDirIterator::Subdirectories);//能筛出所有文件
       while(dir_iterator.hasNext())//遍历目录,找出所有的文件
        {
           QString fuck = dir_iterator.next();
           ++mymapCur[fuck];//把所有文件存到map中
        }
       //找出比上次map中多出的文件
      for( auto &w : mymapCur){//如果在当前的这个文件在上一次文件快照的时候没有出现过，那么就把该文件找出来，认为这个文件是刚产生的
          if(mymapLast.find(w.first) == mymapLast.end()){

            emit fileChange(w.first);//把变化的文件名字作为信号发射出去
            }

         }
       mymapLast = mymapCur;//把当前的map作为上次的map记录

       return;
      }

      emit fileChange(path);//把变化的文件名字作为信号发射出去

}



























