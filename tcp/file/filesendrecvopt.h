#ifndef FILESENDRECVOPT_H
#define FILESENDRECVOPT_H

#include <QObject>
#include <QStringList>

/**
 * @brief The FileSendRecvOpt class
 * �ļ��ķ��ͺͽ��ղ�����
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
public slots:               // �����ķ������ۣ�
    /**
     * @brief ���Ͷ���
     */
    // ����һ��·��
    void addFilePath(QString path);
    // ����һ���ļ�
    void addFileName(QString file);
    // ��շ��Ͷ���
    void clearAllPrepareFile();
    // ͨ���ļ����У���ȥ��һ�����Ͷ��к�ѡ�ļ����У�
    void clearPrepareFileByName(QString name);
    // ֹͣ����
    void stopSend();
    // ��ʼ����
    void startSend();

    /**
     * @brief ���ն���
     */
    // ȷ�Ͻ����ļ�
//    void recvVerify();
    // ���Ϊ�ļ����������Ϊ·����Ϣ
    void fileSavePath(QString path);
    // �����ļ��洢·��
    QString getSavePath();
    // ֹͣ����
    void stopRecv();
signals:
    /**
     * @brief �����źŶ���
     */
    // ��ʼ���͵��ź�
    void signalSendStart();
    // ��ɷ��͵��ź�
    void signalSendSuccess();

    // ���͵��ļ���Ϣ
    void sendInformation(QString type, QString txt);
    // �����ļ��ܽ���
    void sendFilesProgress(int value);
    // ���͵�ǰ�ļ�����
    void sendCurrentFileProgress(int value);
    // �����ļ����ٶ�
    void sendFileSpeed(float mbs);
    /**
     * @brief �����źŶ���
     */
    // �����ļ�ȷ��
    void recvFileVerify();
    // ���յ��ļ���Ϣ
    void recvInformation(QString type, QString txt);
    // �����ļ��ܽ���
    void recvFilesProgress(int value);
    // ���յ�ǰ�ļ�����
    void recvCurrentFileProgress(int value);
    // �����ļ����ٶ�
    void recvFileSpeed(float mbs);

    // ��ʼ�����ź�
    void signalRecvStart();
    // ���յ�Ԥ��������Ϣ
    void signalRecvPreInfo(QStringList lst);
    // ������ɵ��ź�
    void signalRecvSuccess();

    // ���ڷ��͵��ź�
    void signalStopSend();
    void signalStopRecv();

private slots:
    void slotRecvClientOpt(const QString &key, ClientOpt *client);

private:
    class PrivData;
    PrivData *mData;
};

#endif // FILESENDRECVOPT_H
