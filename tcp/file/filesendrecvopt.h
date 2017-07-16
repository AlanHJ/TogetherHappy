#ifndef FILESENDRECVOPT_H
#define FILESENDRECVOPT_H

#include <QObject>
#include <QStringList>

/**
 * @brief The FileSendRecvOpt class
 * 文件的发送和接收操作类
 */
class ClientOpt;
class FileSendRecvOpt : public QObject
{
    Q_OBJECT
public:
    enum InfoType
    {
        normal,
        warning,
        error,
    };

    FileSendRecvOpt();
    virtual~FileSendRecvOpt();
    void setKey(QString key);
    bool isEmpty();
public slots:               // 公共的方法（槽）
    /**
     * @brief 发送定义
     */
    // 增加一个路径
    void addFilePath(QString path);
    // 增加一个文件
    void addFileName(QString file);
    // 清空发送队列
    void clearAllPrepareFile();
    // 通过文件（夹）名去除一个发送队列候选文件（夹）
    void clearPrepareFileByName(QString name);
    // 停止发送
    void stopSend();
    // 开始发送
    void startSend();

    /**
     * @brief 接收定义
     */
    // 确认接收文件
//    void recvVerify();
    // 另存为文件，发送另存为路径信息
    void fileSavePath(QString path);
    // 返回文件存储路径
    QString getSavePath();
    // 停止接收
    void stopRecv();
signals:
    /**
     * @brief 发送信号定义
     */
    // 开始发送的信号
    void signalSendStart();
    // 完成发送的信号
    void signalSendSuccess();

    // 发送的文件信息
    void sendInformation(QString type, QString txt);
    // 发送文件总进度
    void sendFilesProgress(int value);
    // 发送当前文件进度
    void sendCurrentFileProgress(int value);
    // 发送文件的速度
    void sendFileSpeed(float mbs);
    /**
     * @brief 接收信号定义
     */
    // 接收文件确认
    void recvFileVerify();
    // 接收的文件信息
    void recvInformation(QString type, QString txt);
    // 接收文件总进度
    void recvFilesProgress(int value);
    // 接收当前文件进度
    void recvCurrentFileProgress(int value);
    // 接收文件的速度
    void recvFileSpeed(float mbs);

    // 开始接收信号
    void signalRecvStart();
    // 接收的预览数据信息
    void signalRecvPreInfo(QStringList lst);
    // 接收完成的信号
    void signalRecvSuccess();

    // 对内发送的信号
    void signalStopSend();
    void signalStopRecv();

private slots:
    void slotRecvClientOpt(const QString &key, ClientOpt *client);

private:
    class PrivData;
    PrivData *mData;
};

#endif // FILESENDRECVOPT_H
