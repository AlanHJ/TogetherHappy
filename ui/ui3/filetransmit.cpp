#include "filetransmit.h"
#include <QtWidgets>
#include "selfmenu.h"
#include "../../util/thmethod.h"
#include "muiltprogressbar.h"
#include "selfmenu.h"
#include "../notifyui.h"
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include "../../config/configuredata.h"
#include "../../manager/filemanager.h"
#include "../../tcp/socket/socketopt.h"
#include "../../tcp/socket/clientsocketopt.h"

FileTransmit::FileTransmit(QWidget *parent) : QWidget(parent)
{
    sendFlag = false;
    recvFlag = false;
    version = 3.15;
    saveasPath.clear();

    QVBoxLayout *v = new QVBoxLayout();
    v->setContentsMargins(0, 0, 5, 0);

    QWidget *frame1 = new QWidget(this);

    QVBoxLayout *v1 = new QVBoxLayout(frame1);
    v1->setContentsMargins(0, 0, 0, 0);

    QWidget *frame2 = new QWidget(this);

    QVBoxLayout *v2 = new QVBoxLayout(frame2);
    v2->setContentsMargins(0, 0, 0, 5);

    setLayout(v);
    // 传送界面

    FileTree *tree = new FileTree(this);
    tree->setReadOnly(true);
    tree->setHeaderHidden(true);
    // Demonstrating look and feel features
    tree->setAnimated(true);
    tree->setIndentation(0);
    tree->setFixedWidth(160);


    FileTree *tree2 = new FileTree(this);
    tree2->setHeaderHidden(true);
    tree2->setAnimated(true);
    tree2->setIndentation(0);
    tree2->setFixedWidth(160);

    QToolButton *tool = new QToolButton(this);
    tool->setFixedSize(50, 20);
    tool->setText(cn("接收"));
    tool->setPopupMode(QToolButton::MenuButtonPopup);
    SelfMenu *menu = new SelfMenu(tool);
    QAction *a1 = menu->addAction(cn("另存为"));
    tool->setMenu(menu);

    QPushButton *stopRecv = new QPushButton(this);
    stopRecv->setText(cn("终止"));
    stopRecv->setFixedSize(50, 20);

    QHBoxLayout *h1 = new QHBoxLayout();
    h1->setContentsMargins(0, 0, 0, 0);
    h1->addWidget(tool);
    h1->addWidget(stopRecv);

    QPushButton *send = new QPushButton(this);
    send->setText(cn("发送"));
    send->setFixedSize(50, 20);

    QPushButton *stopsend = new QPushButton(this);
    stopsend->setText(cn("终止"));
    stopsend->setFixedSize(50, 20);

    QHBoxLayout *h2 = new QHBoxLayout();
    h2->setContentsMargins(0, 0, 0, 0);
    h2->addWidget(send);
    h2->addWidget(stopsend);

    MuiltProgressBar *bar1 = new MuiltProgressBar(this);
    bar1->setRange(0, 100);
    bar1->setValue(0);
    bar1->setFixedSize(165, 15);

    MuiltProgressBar *bar2 = new MuiltProgressBar(this);
    bar2->setRange(0, 100);
    bar2->setValue(0);
    bar2->setFixedSize(165, 15);

    QLabel *recvLb = new QLabel(this);
    recvLb->setFixedHeight(12);
    recvLb->setText(cn("接收信息显示区..."));
    recvLb->setStyleSheet(cns("QLabel{font-family:微软雅黑;font:8px;color:white;"
                            "}"));

    QLabel *sendLb = new QLabel(this);
    sendLb->setFixedHeight(12);
    sendLb->setText(cn("发送信息显示区..."));
    sendLb->setStyleSheet(cns("QLabel{font-family:微软雅黑;font:8px;color:white;"
                            "}"));

    v1->addWidget(tree, 7, Qt::AlignHCenter);
    v1->addLayout(h1, 1);
    v1->addWidget(bar1, 1, Qt::AlignHCenter);
    v1->addWidget(recvLb, 1, Qt::AlignHCenter | Qt::AlignTop);

    v2->addWidget(tree2, 7, Qt::AlignHCenter);
    v2->addLayout(h2, 1);
    v2->addWidget(bar2, 1, Qt::AlignHCenter);
    v2->addWidget(sendLb, 1, Qt::AlignHCenter | Qt::AlignTop);

    v->addWidget(frame1, 1);
//    v->addSpacing(5);
    v->addWidget(frame2, 1);

    connect(this, &FileTransmit::stopTransmit, this, [=] () {
        emit stopRecv->clicked();
        emit stopsend->clicked();
    });

    connect(this, &FileTransmit::addFile,
            this, [=] (QString path) {
        tree2->addFile(path);
    });

    connect(this, &FileTransmit::parserXml,
            this, [=] (QString xml) {
        frame1->setVisible(true);
        v2->setContentsMargins(0, 5, 0, 5);
        tree->clear();
        tree->parserXml(xml);
        bar1->reset();
        tool->setEnabled(true);
        stopRecv->setEnabled(true);
    });

    connect(send, &QPushButton::clicked, this, [=] () {
        // send files
        if (version < 3.16)
        {
            NotifyUI::ShowNotify(cn("对方哈啤版本太低，不能发送文件"));
            return;
        }
        if (tree2->topLevelItemCount() == 0)
        {
            NotifyUI::ShowNotify(cn("没有需要发送的文件"));
            return;
        }
        if (uid.isEmpty())
        {
            NotifyUI::ShowNotify(cn("没有获取到识别码"));
            return;
        }
        if (sendFlag)
        {
            NotifyUI::ShowNotify(cn("发送正在进行，如需重新发送，请先点击终止！"));
            return;
        }

        QString xml = tree2->toXml();
        QStringList list = tree2->queue();
        FileManager *fm = FileManager::GetInstance();
        fm->insertQueue(uid, list);
        sendLb->setText(cn("等待对方接收..."));
        emit sendXml(xml);
        connect(fm, &FileManager::sendRequested,
                this, [=] (QString uid, SocketOpt *opt) {
            if (this->uid != uid)
                return;
            if (!opt)
                return;

            sendFlag = true;

            connect(stopsend, &QPushButton::clicked, this, [=] () {
                // stop send
                sendFlag = false;
            });

            connect(stopsend, &QPushButton::clicked, opt, &SocketOpt::requested, Qt::DirectConnection);

            connect(opt, &SocketOpt::sendSpeed, this, [=] (float speed) {
                bar2->setInfo(QString("%1MB/s").arg(QString::number(speed, 'f', 2)));
            });
            connect(opt, &SocketOpt::sendProgress, this, [=] (int v1, qreal v2) {
                bar2->setValue(v1);
                bar2->setSecondValue(v2);
            });
            connect(opt, &SocketOpt::sendInformation, this, [=] (QString info) {
                QFontMetrics fm(cn("微软雅黑"));
                sendLb->setText(fm.elidedText(info, Qt::ElideMiddle, 200));
            });
            connect(opt, &SocketOpt::disconnected,
                    this, [=] () {
//                sendLb->setText(cn("断开，终止发送"));
                sendFlag = false;
//                disconnect(stopsend, &QPushButton::clicked, 0, 0);
//                disconnect(opt, &SocketOpt::sendSpeed, 0, 0);
//                disconnect(opt, &SocketOpt::sendProgress, 0, 0);
//                disconnect(opt, &SocketOpt::sendInformation, 0, 0);
//                disconnect(opt, &SocketOpt::disconnected, 0, 0);
//                disconnect(fm, &FileManager::sendRequested, 0, 0);
            });
        });

        send->setEnabled(false);
        QTimer::singleShot(1000, this, [=] () {
            send->setEnabled(true);
        });
    });

    connect(tool, &QToolButton::clicked, this, [=] () {
        // accept files
        if (ip.isEmpty())
        {
            NotifyUI::ShowNotify(cn("没有获取到连接信息"));
            return;
        }
        if (tree->topLevelItemCount() == 0)
        {
            return;
        }
        if (recvFlag)
        {
            NotifyUI::ShowNotify(cn("接收正在进行，如需重新接收，请先点击终止！"));
            return;
        }

        ClientSocketOpt *client = new ClientSocketOpt;        
        QString path = QFileInfo(tree->topLevelItem(0)->text(2)).absolutePath();
        client->setSavePath(path);
        connect(client, &ClientSocketOpt::recvInformation,
                this, [=] (QString info) {
            QFontMetrics fm(cn("微软雅黑"));
            recvLb->setText(fm.elidedText(info, Qt::ElideMiddle, 200));
        });
        connect(client, &ClientSocketOpt::recvProgress,
                this, [=] (int v1, qreal v2) {
            bar1->setValue(v1);
            bar1->setSecondValue(v2);
        });
        connect(client, &ClientSocketOpt::recvSpeed,
                this, [=] (float speed) {
            bar1->setInfo(QString("%1MB/s").arg(QString::number(speed, 'f', 2)));
        });
        connect(stopRecv, &QPushButton::clicked, this, [=] () {
            // stop recv
            recvFlag = false;
        });
        connect(stopRecv, &QPushButton::clicked, client, &ClientSocketOpt::recvRequested, Qt::DirectConnection);

        connect(client, &ClientSocketOpt::connected,
                this, [=] () {
            recvFlag = true;
            ConfigureData *config = ConfigureData::getInstance();
            QByteArray ba = QString("file:%1").arg(config->getUuid()).toLocal8Bit();
            QTimer::singleShot(1000, this, [=] () {
                emit client->senddata(ba);
            });
        });
        connect(client, &ClientSocketOpt::disconnected,
                this, [=] () {
//            recvLb->setText(cn("断开，终止接收"));
            recvFlag = false;
//            disconnect(stopRecv, &QPushButton::clicked, 0, 0);
//            disconnect(client, &ClientSocketOpt::recvInformation, 0, 0);
//            disconnect(client, &ClientSocketOpt::recvProgress, 0, 0);
//            disconnect(client, &ClientSocketOpt::recvSpeed, 0, 0);
//            disconnect(client, &ClientSocketOpt::connected, 0, 0);
//            disconnect(client, &ClientSocketOpt::disconnected, 0, 0);
        });
        emit client->connectHost(ip, HAPPY_LISTEN_PORT);
        tool->setEnabled(false);
        QTimer::singleShot(1000, this, [=] () {
            tool->setEnabled(true);
        });
    });

    connect(a1, &QAction::triggered, this, [=] () {
        QString path = QFileDialog::getExistingDirectory(this);
        if (path.isEmpty())
            NotifyUI::ShowNotify(cn("无效的目录，请重新选择"));
        saveasPath = path;
        for (int i = 0; i < tree->topLevelItemCount(); i++)
        {
            QTreeWidgetItem *item = tree->topLevelItem(i);
            QFileInfo info(item->text(2));
            item->setText(2, saveasPath + "/" + info.fileName());
        }
        emit tool->clicked();
    });

    QFile file;
    file.setFileName(":res/css/tree1.css");
    if (file.open(QIODevice::ReadOnly))
    {
        QByteArray ba = file.readAll();
        tree->setStyleSheet(QTextCodec::codecForLocale()->toUnicode(ba));
        tree2->setStyleSheet(QTextCodec::codecForLocale()->toUnicode(ba));
    }
    file.close();
    file.setFileName(":res/css/toolbtn.css");
    if (file.open(QIODevice::ReadOnly))
    {
        QByteArray ba = file.readAll();
        tool->setStyleSheet(QTextCodec::codecForLocale()->toUnicode(ba));
    }
    file.close();
    file.setFileName(":res/css/button2.css");
    if (file.open(QIODevice::ReadOnly))
    {
        QByteArray ba = file.readAll();
        stopRecv->setStyleSheet(QTextCodec::codecForLocale()->toUnicode(ba));
        send->setStyleSheet(QTextCodec::codecForLocale()->toUnicode(ba));
        stopsend->setStyleSheet(QTextCodec::codecForLocale()->toUnicode(ba));
    }
    file.close();

    frame1->setVisible(false);
}

FileTransmit::~FileTransmit()
{

}

void FileTransmit::paintEvent(QPaintEvent *e)
{
    return QWidget::paintEvent(e);
    int w = width();
    int h = height();
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);
    p.setPen(Qt::white);
    p.drawLine(QPoint(5, h / 2 - 5), QPoint(w - 10, h / 2 - 5));
}
QString FileTransmit::getIp() const
{
    return ip;
}

void FileTransmit::setIp(const QString &value)
{
    ip = value;
}

void FileTransmit::setVersion(float v)
{
    version = v;
}

bool FileTransmit::sendStatus()
{
    return sendFlag;
}

bool FileTransmit::recvStatus()
{
    return recvFlag;
}

QString FileTransmit::getUid() const
{
    return uid;
}

void FileTransmit::setUid(const QString &value)
{
    uid = value;
}


FileTree::FileTree(QWidget *w)
    : QTreeWidget(w)
{
    onlyRead = false;
    setAcceptDrops(true);
    setWordWrap(true);
    setColumnCount(3);
    setColumnWidth(0, 200);
    setColumnWidth(1, 50);
    setColumnWidth(2, 300);

    connect(this, &QTreeWidget::itemDoubleClicked,
            this, [=] (QTreeWidgetItem *it, int /*col*/) {
        QFileInfo info(it->text(2));
        bool b = QDesktopServices::openUrl(QUrl("file:///" + info.filePath()));
        if (!b)
            NotifyUI::ShowNotify(cn("%1打开失败").arg(info.fileName()));
    });
}

FileTree::~FileTree()
{

}

void FileTree::parserXml(QString xml)
{
    QXmlStreamReader reader(xml);
    while (!reader.atEnd())
    {
        if (reader.readNext() == QXmlStreamReader::StartElement)
        {
//            QString txt = reader.name().toString();
            QXmlStreamAttributes attrs = reader.attributes();
            if (attrs.size() >= columnCount())
            {
                QTreeWidgetItem *item = new QTreeWidgetItem(this);
                for (int i = 0; i < attrs.size(); i++)
                {
                    QXmlStreamAttribute attr = attrs.at(i);
                    QString name = attr.name().toString();
                    QString value = attr.value().toString();
                    if (name == "ico")
                    {
                        QByteArray ba = QByteArray::fromBase64(value.toLocal8Bit());
                        QPixmap pix(QSize(32, 32));
                        pix.loadFromData(ba, "png");
                        QIcon ico = QIcon(pix);
                        item->setIcon(0, ico);
                    }
                    else if (name == "name")
                    {
                        item->setText(0, value);
                    }
                    else if (name == "sizeortype")
                    {
                        item->setText(1, value);
                    }
                    else if (name == "path")
                    {
                        QFileInfo info(value);
                        ConfigureData *config = ConfigureData::getInstance();
                        QString path = config->getIni("download");
                        item->setText(2, path + "/" + info.fileName());
                    }
                    else
                    {
                        item->setText(i - 1, value);
                    }
                }
            }
        }
    }
}

void FileTree::addFile(QString path)
{
    QFileInfo info(path);
    if (!info.exists())
    {
        return;
    }

    // check exist item
    for (int i = 0; i < topLevelItemCount(); i++)
    {
        QTreeWidgetItem *it = topLevelItem(i);
        if (it->text(2) == info.filePath())
        {
            NotifyUI::ShowNotify(cn("文件已经存在"));
            return;
        }
    }

    QTreeWidgetItem *item = new QTreeWidgetItem(this);
    QFileIconProvider provider;
    QIcon ico = provider.icon(info);
    item->setIcon(0, ico);
    item->setText(0, info.fileName());
    if (info.isFile())
    {
        item->setText(1, QString("%1MB").arg(QString::number(info.size() / 1024.0 / 1024.0, 'f', 2)));
    }
    else if (info.isDir())
    {
        item->setText(1, cn("文件夹"));
    }
    else
    {
        item->setText(1, cn("未知"));
    }
    item->setText(2, info.filePath());
}

void FileTree::setReadOnly(bool b)
{
    onlyRead = b;
}

QString FileTree::toXml()
{
    QString xml;
    QXmlStreamWriter writer(&xml);
    writer.writeStartDocument();
    writer.writeStartElement("files");
    for (int i = 0; i < topLevelItemCount(); i++)
    {
        QTreeWidgetItem *item = topLevelItem(i);
        writer.writeStartElement(QString("file%1").arg(i));
        QXmlStreamAttributes attrs;
        QIcon ico = item->icon(0);
        QPixmap pix = ico.pixmap(QSize(32, 32));
        QByteArray bytes;
        QBuffer buffer(&bytes);
        buffer.open(QIODevice::WriteOnly);
        pix.save(&buffer, "PNG");
        buffer.close();
        attrs.append("ico", bytes.toBase64());
        attrs.append(QString("name"), item->text(0));
        attrs.append(QString("sizeortype"), item->text(1));
        attrs.append(QString("path"), item->text(2));
        writer.writeAttributes(attrs);
        writer.writeEndElement();
    }
    writer.writeEndElement();
    writer.writeEndDocument();
    return xml;
}

QStringList FileTree::queue()
{
    QStringList list;
    for (int i = 0; i < topLevelItemCount(); i++)
    {
        QTreeWidgetItem *item = topLevelItem(i);
        list << item->text(2);
    }
    return list;
}

void FileTree::dragEnterEvent(QDragEnterEvent *e)
{
    if (onlyRead)
        return;
    const QMimeData *md = e->mimeData();
    if (!md)
    {
        return QTreeWidget::dragEnterEvent(e);
    }
    if (md->hasUrls())
    {
        QList<QUrl> lst = md->urls();
        auto it = lst.begin();
        while (it != lst.end())
        {
            QUrl url = *it;
            QString path = url.path();
            addFile(path.right(path.length() - 1));
            ++it;
        }
    }
}

void FileTree::contextMenuEvent(QContextMenuEvent *e)
{
    SelfMenu menu;
    QTreeWidgetItem *it = itemAt(e->pos());
    if (it)
    {
        QFileInfo info(it->text(2));
        if (!onlyRead) {
            connect(menu.addAction(cn("移除")), &QAction::triggered,
                                   this, [&] () {
                delete it;
                it = nullptr;
            });
        }
        connect(menu.addAction(cn("打开文件")), &QAction::triggered,
                               this, [=] () {
            bool b = QDesktopServices::openUrl(QUrl("file:///" + info.filePath()));
            if (!b)
                NotifyUI::ShowNotify(cn("%1打开失败").arg(info.fileName()));
//            QProcess::execute("explorer " + info.filePath()/*.replace(":", ":/")*/.replace("/", "\\"));
        });
        connect(menu.addAction(cn("打开目录")), &QAction::triggered,
                               this, [=] () {
            bool b = QDesktopServices::openUrl(QUrl("file:///" + info.absolutePath()));
            if (!b)
                NotifyUI::ShowNotify(cn("目录%1打开失败").arg(info.fileName()));
        });
    }
    if (topLevelItemCount() != 0)
    {
        connect(menu.addAction(cn("清空队列")), &QAction::triggered,
                               this, [=] () {
                this->clear();
        });
    }
    if (!onlyRead)
    {
        connect(menu.addAction(cn("打开下载目录")), &QAction::triggered,
                               this, [&] () {
            ConfigureData *config = ConfigureData::getInstance();
            QString path = config->getIni("download");
            bool b = QDesktopServices::openUrl(QUrl("file:///" + path));
            if (!b)
                NotifyUI::ShowNotify(cn("目录%1打开失败").arg(path));
        });
    }
    menu.exec(QCursor::pos());
}


//FileModel::FileModel(QObject *obj)
//{

//}

//FileModel::~FileModel()
//{

//}

//QModelIndex FileModel::index(int row, int column, const QModelIndex &parent) const
//{

//}

//QModelIndex FileModel::parent(const QModelIndex &child) const
//{

//}

//int FileModel::rowCount(const QModelIndex &parent) const
//{

//}

//int FileModel::columnCount(const QModelIndex &parent) const
//{

//}

//QVariant FileModel::data(const QModelIndex &index, int role) const
//{

//}
