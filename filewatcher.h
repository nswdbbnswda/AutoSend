#ifndef FILEWATCHER_H
#define FILEWATCHER_H
#include<QDirIterator>
#include<QFileSystemWatcher>
#include<map>
#include<queue>
#include<QDebug>
#include<string>
#include"pathremake.h"

//单例模式
class FileWatcher : public PathRemake {
    Q_OBJECT
public:
    static FileWatcher *getInstance()
    {
        static FileWatcher *m_pInstance;
        if(m_pInstance == NULL){
            m_pInstance = new  FileWatcher();
        }           return m_pInstance;
    }
    qint64 GetTotalFileSize();
    void GetFileList(const QString &path,std::queue<QString> &saveQueue);
    void GetFileList(const std::queue<QString> & _pathList,std::map<QString,QString> &saveMap,std::queue<QString> &saveQueue,std::map<QString,QString> &fileNameMap);
    virtual  ~FileWatcher();
private:
    FileWatcher();
private:
    qint64 TotalFileSize;//所有文件总大小

};

#endif // FILEWATCHER_H
