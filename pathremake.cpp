#include "pathremake.h"
#include<QDebug>
#include<QDirIterator>


//输入用于遍历的路径
PathRemake::PathRemake()
{

}

//析构函数
PathRemake::~PathRemake()
{

}


//D:/temp/1.txt -> D:/temp/ 或者 C:/data/ms -> C:/data/
QString  PathRemake::GetPrePath(const QString &_path)
{
    QFileInfo fileInfo(_path);
    if(!fileInfo.exists()){ qDebug()<<"路径不合法SegmentPath()"; return "";}
    if(fileInfo.isFile()){//是一个文件
        return fileInfo.absolutePath() + "/";  //G:/DE/  返回文件名字之前的路径
    }else if(fileInfo.isDir()){//如果是一个文件夹
        int pos;
        QString prePath;
        if(-1 == (pos = _path.lastIndexOf("/"))){ qDebug()<<"没有找到路径分割符(PathRemake)";}// 找到最后一个"/"所在位置
        prePath = _path.left(pos + 1);  //G:/min  G:/
        return prePath;
    }
}


//获得去掉目录的文件名字（保留目录结构) 如果要发送的是一个文件夹,D:/data  ，其中1.txt是data中的文件，那么返回值就是data/1.txt
QString PathRemake::GetSendFilePath(const QString &fullFilePath,const QString &dirPath)
{
    QString path = fullFilePath.right(fullFilePath.length() - GetPrePath(dirPath).length());
    return path;
}



//判断一个目录是否存在
bool PathRemake::IsDirExist(const QString &path)
{
    QDir dir(path);//实例化一个目录对象
    if(dir.exists()){return true;}//存在
    else{  qDebug()<<"目录不存在"; return false;}//不存在
}












