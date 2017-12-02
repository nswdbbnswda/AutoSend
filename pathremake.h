#ifndef PATHREMAKE_H
#define PATHREMAKE_H
#include <QObject>

class PathRemake : public QObject
{
    Q_OBJECT
public:
    explicit PathRemake(QObject *parent = 0);
             PathRemake(const QString &_folderPath);
    virtual  ~PathRemake();
    QString  GetFolerPath();//获取要遍历的文件夹全路径 例如:  G:/min
    QString  GetFolerName();//获取文件夹名
    QString  GetPreFolerPath();//获取文件夹名之前的路径  G:/
    QString  GetFolerFilePath(QString fullFilePath);//获得文件路径，不包括 文件夹之前的 部分 D:/DATA/1.txt   ->   DATA/1.txt
    QString  TransToFullPath(QString folerFilePath);//切换到  硬盘路径  DATA/1.txt -> D:/DATA/1.txt

private:
    QString  folderPath;//要遍历的问文件夹路径
    QString  folderPrePath;//文件夹名 之前的部分 G:/
    QString  folerName;//文件夹名 min

signals:

public slots:

};

#endif // PATHREMAKE_H
