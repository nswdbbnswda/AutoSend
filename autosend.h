#ifndef AUTOSEND_H
#define AUTOSEND_H
#include<QString>

#define IPMSG_DEFAULT_IOBUFMAX		(1024 * 1024)		// 1MB
#define IPMSG_DEFAULT_WAITTIMEMAX   600000 //10分钟
#define IPMSG_DEFAULT_QTINERBUFFER   1048576000 //1000M
#define IPMSG_DEFAULT_WAITFORREADYREAD 30000 //30秒没数据过来，就认为断线了

struct BreakPoint{//断点结构
    QString filename;//断点文件
    qint64  pos;//文件断点位置
};

#endif // AUTOSEND_H
