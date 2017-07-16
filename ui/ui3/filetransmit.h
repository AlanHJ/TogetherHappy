#ifndef FILETRANSMIT_H
#define FILETRANSMIT_H

#include <QWidget>
#include <QTreeWidget>

//class FileModel : public QAbstractItemModel
//{
//    Q_OBJECT

    // QAbstractItemModel interface
//public:
//    FileModel(QObject *obj = 0);
//    ~FileModel();
//    QModelIndex index(int row, int column, const QModelIndex &parent) const;
//    QModelIndex parent(const QModelIndex &child) const;
//    int rowCount(const QModelIndex &parent) const;
//    int columnCount(const QModelIndex &parent) const;
//    QVariant data(const QModelIndex &index, int role) const;
//};

class FileTree : public QTreeWidget
{
    Q_OBJECT
public:
    FileTree(QWidget *w = 0);
    ~FileTree();
public slots:
    void addFile(QString path);
    void setReadOnly(bool b);
    void parserXml(QString xml);
    QString toXml();
    QStringList queue();
    // QWidget interface
protected:
    void dragEnterEvent(QDragEnterEvent *e);

private:
    bool onlyRead;

    // QWidget interface
protected:
    void contextMenuEvent(QContextMenuEvent *e);
};

class FileTransmit : public QWidget
{
    Q_OBJECT
public:
    explicit FileTransmit(QWidget *parent = 0);
    ~FileTransmit();

    QString getUid() const;
    void setUid(const QString &value);

    QString getIp() const;
    void setIp(const QString &value);

    void setVersion(float v);

    bool sendStatus();

    bool recvStatus();

signals:
    void stopTransmit();
    void addFile(QString path);
    void parserXml(QString xml);
    void sendXml(QString xml);
    // QWidget interface
protected:
    void paintEvent(QPaintEvent *e);

private:
    QString saveasPath;
    QString uid;
    QString ip;
    double version;
    bool sendFlag;
    bool recvFlag;
};

#endif // FILETRANSMIT_H
