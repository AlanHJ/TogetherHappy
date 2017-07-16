#include "optionnetwork.h"
#include "../../util/thmethod.h"
#include "../../config/configuredata.h"
#include "../notifyui.h"
#include <QtWidgets>
#include <QtNetwork>
#include "optionsui.h"

OptionNetwork::OptionNetwork(QWidget *parent) : OptionWidget(parent)
{
    QCheckBox *box1 = new QCheckBox(QStringLiteral("�Զ���ȡIP��ַ"), this);
    box1->setFixedSize(CalcStrByFont(box1->text()).width(), 30);
    QLabel *lbl = new QLabel(this);
    lbl->setText("127.0.0.1");
    lbl->setStyleSheet(QStringLiteral("font-family:΢���ź�;font:12px;color:white;"));

    QLineEdit *edit = new QLineEdit(this);
    edit->setFixedWidth(200);
    edit->setPlaceholderText(QStringLiteral("�ֶ�����IP��ַ,��ʽ��:127.0.0.1"));

    QHBoxLayout *h = new QHBoxLayout();
    h->setContentsMargins(0, 0, 0, 0);
    h->setSpacing(0);
    h->addWidget(box1, 2);
    h->addWidget(lbl, 1);

    QHBoxLayout *h1 = new QHBoxLayout();
    h1->setContentsMargins(3, 0, 0, 0);
    h1->setSpacing(0);
    h1->addWidget(edit);
    h1->addStretch(1);

    QVBoxLayout *v = new QVBoxLayout(this);
    v->setContentsMargins(3, 3, 3, 3);
    v->setSpacing(0);
    v->addLayout(h, 1);
    v->addLayout(h1, 1);
    v->addStretch(8);

    QFile file;
    file.setFileName(":res/css/checkbox.css");
    if (file.open(QIODevice::ReadOnly))
    {
        QByteArray ba = file.readAll();
        box1->setStyleSheet(QTextCodec::codecForLocale()->toUnicode(ba));
    }
    file.close();
    file.setFileName(":res/css/lineedit.css");
    if (file.open(QIODevice::ReadOnly))
    {
        QByteArray ba = file.readAll();
        edit->setStyleSheet(QTextCodec::codecForLocale()->toUnicode(ba));
    }
    file.close();


    ConfigureData *conf = ConfigureData::getInstance();
    // load data
    box1->setChecked(conf->IsTrue("ip_get"));
    lbl->setText(conf->getIni("ip"));
    if (!box1->isChecked())
    {
        edit->setText(conf->getIni("ip"));
    }
    connect(box1, &QCheckBox::toggled, this, [=] (bool b) {
        edit->setEnabled(!b);
        conf->setIni("ip_set", b);
        if (b)
        {
            QString ipStr;
            QList<QNetworkInterface> interfacelst = QNetworkInterface::allInterfaces();
            for (QNetworkInterface face : interfacelst)
            {
                if (face.flags() == (QNetworkInterface::IsUp |
                                     QNetworkInterface::IsRunning |
                                     QNetworkInterface::CanBroadcast |
                                     QNetworkInterface::CanMulticast))
                {
                    if (!face.humanReadableName().contains("VMw", Qt::CaseInsensitive)
                            && !face.humanReadableName().contains("Vir", Qt::CaseInsensitive))
                    {
                        for (QNetworkAddressEntry entry : face.addressEntries())
                        {
                            if (entry.ip().protocol() == QAbstractSocket::IPv4Protocol)
                            {
                                qDebug() << face.hardwareAddress(); // MAC address
                                ipStr = entry.ip().toString();
                                break;
                            }
                        }
                        break;
                    }
                }
            }
            if (ipStr.isEmpty())
            {
                NotifyUI::ShowNotify(cn("�Զ���ȡʧ��,�볢���ֶ�����"));
            }
            else
            {
                NotifyUI::ShowNotify(cn("�Զ���ȡ�ɹ�,���IPΪ:%1").arg(ipStr));
                lbl->setText(ipStr);
            }
        }
    });


    OptionsUI *opt = dynamic_cast<OptionsUI *>(parent);
    connect(opt, &OptionsUI::apply, this, [=] (int tab) {
        if (tab != index)
            return;
        NotifyUI::ShowNotify(cn("��������Ч"));
        QString ip;
        if (box1->isChecked())
        {
            ip = lbl->text();
        }
        else
        {
            ip = edit->text();
        }
        conf->setIni("ip", ip);

    });
    connect(opt, &OptionsUI::confirm, this, [=] (int tab) {
        if (tab != index)
            return;
        emit opt->apply(tab);
    });
}

OptionNetwork::~OptionNetwork()
{

}

