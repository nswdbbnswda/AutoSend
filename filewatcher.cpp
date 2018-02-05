#include "filewatcher.h"
#include<iostream>


FileWatcher::FileWatcher()
{
    TotalFileSize = 0;
}

FileWatcher::~FileWatcher()
{

}





//根据给定的路径遍历文件夹,并且把遍历到的文件列表存到队列中
void FileWatcher::GetFileList(const QString &path, std::queue<QString> &saveQueue)
{
    if(!IsDirExist(path)) return;//判断目录是否存在
    QStringList filters; //获取所选文件类型过滤器
    filters<<QString("*.*");
    QDirIterator dir_iterator(path,QDir::Files | QDir::NoSymLinks|QDir::Hidden, QDirIterator::Subdirectories);//能筛出所有文件
    while(dir_iterator.hasNext())//遍历目录
    {
        QString temp = dir_iterator.next();
        saveQueue.push(temp);//把遍历出来的文件都存储到队列中
    }

}




//根据路径队列得到全路径文件队列和文件查找字典
void FileWatcher::GetFileList(const std::queue<QString> &_pathList, std::map<QString, QString> &saveMap, std::queue<QString> &saveQueue,std::map<QString,QString> &fileNameMap)
{
    std::queue<QString>  pathList = _pathList;//获得路径队列
    QString path;
    while(pathList.size()){
        //拿到一个路径并且判断该路径的有效性
        path =  pathList.front();
        QFileInfo fileInfo(path);
        if(!fileInfo.exists()){ qDebug()<<"路径不合法SegmentPath()"; return;}



        //判断路径类型
        if(fileInfo.isFile()){//是一个文件

            //累加文件大小
            TotalFileSize += fileInfo.size();

            saveQueue.push(path);//把文件全路径加入到文件队列
            saveMap[fileInfo.fileName()] = path; //把文件名字和全路径加入到字典中
            fileNameMap[path] = fileInfo.fileName();
        }else if(fileInfo.isDir()){//是一个文件夹

            //fileInfo.size();

            if(!IsDirExist(path)) return;//判断目录是否存在
            QStringList filters; //获取所选文件类型过滤器
            filters<<QString("*.*");
            QDirIterator dir_iterator(path,QDir::Files | QDir::NoSymLinks|QDir::Hidden, QDirIterator::Subdirectories);//能筛出所有文件
            while(dir_iterator.hasNext())//遍历目录
            {
                QString temp = dir_iterator.next();
                //将文件夹内的文件大小进行累加
                QFileInfo dirFileInfo(temp);
                TotalFileSize += dirFileInfo.size();

                saveQueue.push(temp);//把遍历出来的文件都存储到队列中
                saveMap[GetSendFilePath(temp,path)] = temp;//文件夹中的每一个文件都在字典中
                fileNameMap[temp] = GetSendFilePath(temp,path);
            }
        }

        pathList.pop();

    }
    qDebug()<<"File Numbler:"<<saveQueue.size();
}



//获得文件总尺寸大小
qint64 FileWatcher::GetTotalFileSize()
{
    return TotalFileSize;
}



















