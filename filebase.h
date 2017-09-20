#ifndef FILEBASE_H
#define FILEBASE_H
#include<QObject>
#include<QString>
#include<string>


class FileBase : public QObject{
    Q_OBJECT
public:

    FileBase(const std::string &path);
    FileBase() = default;
    virtual ~FileBase();
    void setPath(const std::string &path){ strPath = path;}//设置路径
    std::string& getPath(){ return strPath;}//获取路径
protected:
    std::string strPath;//路径
};

#endif // FILEBASE_H
