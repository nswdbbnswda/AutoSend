#include "pathremake.h"
#include<QDebug>

PathRemake::PathRemake(QObject *parent) :
    QObject(parent)
{


}

//输入用于遍历的路径
PathRemake::PathRemake(const QString &_folderPath)
{
    int pos;
    folderPath = _folderPath;
    if(-1 == (pos = folderPath.lastIndexOf("/"))){ qDebug()<<"没有找到路径分割符(PathRemake)";}   // 找到最后一个"/"所在位置
    folderPrePath = folderPath.left(pos + 1);  //G:/min  G:/
    folerName = folderPath.right( folderPath.length() - folderPrePath.length());
}


PathRemake::~PathRemake()
{

}

QString PathRemake::GetFolerPath()
{
    return folderPath;
}



QString PathRemake::GetFolerName()
{
    return folerName;
}

QString PathRemake::GetPreFolerPath()
{
    return folderPrePath;
}

QString PathRemake::GetFolerFilePath(QString fullFilePath)
{
    QString path = fullFilePath.right(fullFilePath.length() - folderPrePath.length());
    return path;
}



QString PathRemake::TransToFullPath(QString folerFilePath)
{
    return folderPrePath + folerFilePath;
}





