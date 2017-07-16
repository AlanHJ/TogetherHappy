#include "personchat.h"
#include "friendinfo.h"
#include "filetransmit.h"
#include "../../data/json/broadcastjsondata.h"
#include "../../thwidgets/happytextedit.h"
#include "../../data/messagedata.h"
#include "../../data/messagerichtext.h"
#include "../../gsoap/call/gsoapcalloperation.h"
#include "../../gsoap/gsoapmessageinstance.h"
#include "../../data/messagechardata.h"
#include "../../config/configuredata.h"
#include "../notifyui.h"
#include "msgbox.h"
#include <QtWidgets>

PersonChat::PersonChat(QWidget *w) : Chat(true, w)
{
    jsondata = nullptr;
    shakeabled = true;
    init();
    connect(this, &PersonChat::windowTitleChanged,
            this, [=] () {info->updateMemoName();});
}

PersonChat::~PersonChat()
{
    if (jsondata)
    {
        delete jsondata;
        jsondata = nullptr;
    }
}

void PersonChat::setJsonData(BroadcastJsonData *data)
{
    this->jsondata = data;
    info->setJsondata(this->jsondata->getUid());
    transmit->setUid(jsondata->getUid());
    transmit->setIp(jsondata->getGsoapip());
    transmit->setVersion(jsondata->getVersion());
}

void PersonChat::setRecever(BroadcastReceverJson *recever)
{
    info->setRecever(recever);
}

void PersonChat::init()
{
    QPushButton *personBtn = new QPushButton(this);
    personBtn->setToolTip(QStringLiteral("��Ϣ"));
    personBtn->setFixedSize(35, 30);
    personBtn->setCheckable(true);
    transmitBtn = new QPushButton(this);
    transmitBtn->setToolTip(QStringLiteral("�ļ�����"));
    transmitBtn->setFixedSize(35, 30);
    transmitBtn->setCheckable(true);

    stackBtnGroup->addButton(personBtn, 0);
    stackBtnGroup->addButton(transmitBtn, 1);
    topBar->addWidget(personBtn);
    topBar->addWidget(transmitBtn);

    info = new FriendInfo(stack);
    info->setMouseTracking(true);
    transmit = new FileTransmit(this);
    transmit->setMouseTracking(true);
    stack->addWidget(info);
    stack->addWidget(transmit);

    personBtn->setChecked(true);

    QFile file;
    file.setFileName(":res/css/button.css");
    if (file.open(QIODevice::ReadOnly))
    {
        QString str = file.readAll();
        personBtn->setStyleSheet(str.arg("chat_person").arg("chat_person").arg("chat_person"));
        transmitBtn->setStyleSheet(str.arg("file_transport").arg("file_transport").arg("file_transport"));
    }
    file.close();

    // connect
    sender = GsoapCallOperation::Sender();
    connect(sender, &GsoapCallOperation::signalChatMsgSendFail,
            this, [=] (QString uid, QString er) {
        if (!jsondata)
            return;
        if (uid != jsondata->getUid())
            return;
        inserHtml(cn("����ʧ�ܣ�������\n") + er, cn("��ơ��ʾ"), Qt::red);
    });

    // set shake time
    connect(shakeBtn, &QToolButton::clicked, this, [=] () {
        // send shake
        if (!shakeabled)
        {
            inserHtml(cn("��̫Ƶ����, �������ܲ��˵�..."),
                      cn("��ơ��ʾ"), Qt::yellow);
            return;
        }
        shakeabled = false;
        emit starkShake();
        inserHtml(cn("�㶶�˶Է�һ��"),
                  cn("��ơ��ʾ"), Qt::yellow);
        emit sender->signalChatShakeInterface(jsondata->getUid(),
                                              jsondata->getGsoapip());
    });

    connect(shakeReject, &QAction::triggered, this, [=] (bool b) {
        emit shakeRejectStatusChanged(b);
    });

    connect(edit, &HappyTextEdit::signalGetPath,
            this, [=] (QString path) {
        stack->setCurrentIndex(1);
        emit transmit->addFile(path);
        emit hideBtn->toggled(true);
        transmitBtn->setChecked(true);
    });

    connect(transmit, &FileTransmit::sendXml,
            this, [=] (QString xml) {
        // send previously file msg
        if (!jsondata)
        {
            NotifyUI::ShowNotify(cn("�޷����ͣ�ȱ���ؼ�����"));
            return;
        }
        MessageData msg;
        MessageCharData *str = new MessageCharData;
        str->setContent(xml.toLocal8Bit());
        str->setType(FilePrev);
        msg.setUid(jsondata->getUid());
        msg.setDatatype(str->datatype());
        msg.setContent(str);
        emit sender->signalChatOneInterface(jsondata->getUid(),
                                            jsondata->getGsoapip(),
                                            msg);
    });
}


void PersonChat::sendMsg()
{
    QString html = edit->toHtml();
    inserHtml(html, cn("��"));

    if (!jsondata)
        return;
    MessageData msg;
    MessageRichText *content = new MessageRichText();
    content->setMessageHtml(html);
    msg.setUid(jsondata->getUid());
    msg.setDatatype(content->datatype());
    msg.setContent(content);

//    qDebug() << jsondata->getGsoapip();
    emit sender->signalChatOneInterface(jsondata->getUid(),
                                        jsondata->getGsoapip(),
                                        msg);
}

void PersonChat::recvMsg(QString html, QString src)
{
    inserHtml(html, src, Qt::darkBlue);
    qApp->alert(this, 0);
    isAlert = true;
    lastRecvTime = QDateTime::currentDateTime().toMSecsSinceEpoch();
}

void PersonChat::setShakeEnable(bool b)
{
    shakeabled = b;
}

void PersonChat::closeEvent(QCloseEvent *e)
{
    emit chatClose();
    ConfigureData *config = ConfigureData::getInstance();
    config->setIni("chat_w", this->width());
    config->setIni("chat_h", this->height());
    bool b1 = transmit->sendStatus();
    bool b2 = transmit->recvStatus();
    if (b1 || b2)
    {
        QString str = cn("���ڴ����ļ������Ҫ�رմ���(��ֹͣ����)ô��");
        if (b1)
            str = cn("���ڷ����ļ������Ҫ�رմ���(��ֹͣ����)ô��");
        if (b2)
            str = cn("���ڽ����ļ������Ҫ�رմ���(��ֹͣ����)ô��");
        if (b1 && b2)
            str = cn("���ڴ����ļ������Ҫ�رմ���(��ֹͣ����)ô��");
        int ret = MsgBox::ShowMsgBox(cn("����"),
                                     str,
                                     cn("�ر�"), cn("���"));
        if (ret == 0)
        {
            emit transmit->stopTransmit();
            e->accept();
            this->deleteLater();
        }
        else
            e->ignore();
    }
    else
    {
        e->accept();
        this->deleteLater();
    }
}


void PersonChat::execRecvCmd(RecvCmd cmd, QVariant var)
{
    if (cmd == shake)
    {
        inserHtml(cn("�Է�������һ��"),
                  cn("��ơ��ʾ"), Qt::yellow);
        qApp->alert(this, 0);
        if (shakeReject->isChecked())
            return;
        this->showNormal();
        this->activateWindow();
        // ������
        QTimer::singleShot(100, [=] () {
            int x = pos().x();
            int y = pos().y();
            int algh = 5;
            QTime time;
            time.start();
            while (time.elapsed() < 1000)
            {
                move(x - algh, y);
                thread()->msleep(25);
                move(x, y - algh);
                thread()->msleep(25);
                move(x + algh, y);
                thread()->msleep(25);
                move(x, y + algh);
                thread()->msleep(25);
                showNormal();
                activateWindow();
            }
            move(x, y);
        });
    }
    if (cmd == fileprev)
    {
        inserHtml(cn("�Է��������ļ��������ұ߽���ѡ�����"),
                  cn("��ơ��ʾ"), Qt::yellow);
        this->showNormal();
        this->activateWindow();
        stack->setCurrentIndex(1);
        emit hideBtn->toggled(true);
        transmitBtn->setChecked(true);
        transmit->parserXml(var.toString());
    }
}

void PersonChat::setRejectShake(bool b)
{
    shakeReject->setChecked(b);
}


void PersonChat::setChatTitle(QString content, QString ico)
{
    Chat::setChatTitle(content, ico);
    setWindowTitle(content.remove(cn("��")).remove(cn("������")));
}
