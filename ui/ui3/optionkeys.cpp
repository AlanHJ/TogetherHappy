#include "optionkeys.h"
#include <QtWidgets>
#include <QKeyEvent>
#include "../../util/thmethod.h"
#include "../../config/configuredata.h"
#include "../notifyui.h"
#include "../hotkeyregiter.h"
#include "optionsui.h"

OptionKeys::OptionKeys(QWidget *parent)
    : OptionWidget(parent)
{
    QVBoxLayout *v = new QVBoxLayout(this);
    v->setContentsMargins(3, 3, 3, 3);
    v->setSpacing(0);
    QCheckBox *box1 = new QCheckBox(QStringLiteral("ÆôÓÃ¿ì½Ý¼ü"), this);
    box1->setFixedSize(CalcStrByFont(box1->text()).width(), 30);
    v->addWidget(box1, 1, Qt::AlignLeft);

    QHBoxLayout *h1 = new QHBoxLayout();
    h1->setContentsMargins(0, 0, 0, 0);
    QLabel *lbl1 = new QLabel(QStringLiteral("½ØÍ¼²Ù×÷"), this);
    lbl1->setFixedWidth(180);
    lbl1->setStyleSheet(QStringLiteral("font-family:Î¢ÈíÑÅºÚ;font:12px;color:white;"));
    KeyLineEdit *key1 = new KeyLineEdit(this);
    h1->addWidget(lbl1, 1);
    h1->setSpacing(5);
    h1->addWidget(key1, 4);

    QHBoxLayout *h2 = new QHBoxLayout();
    h2->setContentsMargins(0, 0, 0, 0);
    QLabel *lbl2 = new QLabel(QStringLiteral("µ¯³öÏûÏ¢(ÎÞÏûÏ¢Ê±ÏÔÊ¾Ö÷½çÃæ)"), this);
    lbl2->setFixedWidth(180);
    lbl2->setStyleSheet(QStringLiteral("font-family:Î¢ÈíÑÅºÚ;font:12px;color:white;"));
    KeyLineEdit *key2 = new KeyLineEdit(this);
    h2->addWidget(lbl2, 1);
    h2->setSpacing(5);
    h2->addWidget(key2, 4);

    QHBoxLayout *h3 = new QHBoxLayout();
    h3->setContentsMargins(0, 0, 0, 0);
    QLabel *lbl3 = new QLabel(QStringLiteral("ËøÆÁ"), this);
    lbl3->setFixedWidth(180);
    lbl3->setStyleSheet(QStringLiteral("font-family:Î¢ÈíÑÅºÚ;font:12px;color:white;"));
    KeyLineEdit *key3 = new KeyLineEdit(this);
    h3->addWidget(lbl3, 1);
    h3->setSpacing(5);
    h3->addWidget(key3, 4);


    QHBoxLayout *h4 = new QHBoxLayout();
    h4->setContentsMargins(0, 0, 0, 0);
    QLabel *lbl4 = new QLabel(QStringLiteral("¹Ø±ÕÁÄÌì´°¿Ú(½öÖÃÇ°´°¿Ú)"), this);
    lbl4->setFixedWidth(180);
    lbl4->setStyleSheet(QStringLiteral("font-family:Î¢ÈíÑÅºÚ;font:12px;color:white;"));
    KeyLineEdit *key4 = new KeyLineEdit(this);
    h4->addWidget(lbl4, 1);
    h4->setSpacing(5);
    h4->addWidget(key4, 4);


    QHBoxLayout *h5 = new QHBoxLayout();
    h5->setContentsMargins(0, 0, 0, 0);
    QLabel *lbl5 = new QLabel(QStringLiteral("ÏÔÊ¾ÐèÒª»Ø¸´µÄ´°¿Ú"), this);
    lbl5->setFixedWidth(180);
    lbl5->setStyleSheet(QStringLiteral("font-family:Î¢ÈíÑÅºÚ;font:12px;color:white;"));
    KeyLineEdit *key5 = new KeyLineEdit(this);
    h5->addWidget(lbl5, 1);
    h5->setSpacing(5);
    h5->addWidget(key5, 4);

    QHBoxLayout *h6 = new QHBoxLayout();
    h6->setContentsMargins(0, 0, 0, 0);
    QLabel *lbl6 = new QLabel(QStringLiteral("½«µ±Ç°´°¿ÚÒþ²Øµ½ÈÎÎñÀ¸"), this);
    lbl6->setFixedWidth(180);
    lbl6->setStyleSheet(QStringLiteral("font-family:Î¢ÈíÑÅºÚ;font:12px;color:white;"));
    KeyLineEdit *key6 = new KeyLineEdit(this);
    h6->addWidget(lbl6, 1);
    h6->setSpacing(5);
    h6->addWidget(key6, 4);

    v->addLayout(h1, 1);
    v->addLayout(h2, 1);
    v->addLayout(h3, 1);
    v->addLayout(h4, 1);
    v->addLayout(h5, 1);
    v->addLayout(h6, 1);
    v->addStretch(1);

    QFile file;
    file.setFileName(":res/css/checkbox.css");
    if (file.open(QIODevice::ReadOnly))
    {
        QByteArray ba = file.readAll();
        box1->setStyleSheet(QTextCodec::codecForLocale()->toUnicode(ba));
    }
    file.close();

    HotkeyRegiter *hot = HotkeyRegiter::GetInstance();
    ConfigureData *config = ConfigureData::getInstance();
    box1->setChecked(config->IsTrue("key_enable"));
    key1->setText(config->getIni("shot_key").toUpper());
    key2->setText(config->getIni("msg_key").toUpper());
    key3->setText(config->getIni("lock_key").toUpper());

    QString keyStr = config->getIni("close_key").toUpper();
    if (keyStr.isEmpty())
        keyStr = "ALT+C";
    key4->setText(keyStr);

    keyStr = config->getIni("show_need_replay").toUpper();
    if (keyStr.isEmpty())
        keyStr = "ALT+S";
    key5->setText(keyStr);

    keyStr = config->getIni("hide_active").toUpper();
    if (keyStr.isEmpty())
        keyStr = "ALT+A";
    key6->setText(keyStr);


    connect(box1, &QCheckBox::toggled, this, [=] (bool b) {
        config->setIni("key_enable", b);
    });

    OptionsUI *opt = dynamic_cast<OptionsUI *>(parent);
    connect(opt, &OptionsUI::apply, this, [=] (int tab) {
        if (tab != index)
            return;
        QString k1 = key1->text();
        if (k1 != config->getIni("shot_key"))
        {
            hot->unRegister(1);
            if (hot->RegisterHotkey(1, k1))
                config->setIni("shot_key", k1);
            else
            {
                NotifyUI::Show3sNotify(cn("½ØÍ¼¿ì½Ý¼ü×¢²áÊ§°Ü"));
            }
        }

        QString k2 = key2->text();
        if (k2 != config->getIni("msg_key"))
        {
            hot->unRegister(0);
            if (hot->RegisterHotkey(0, k2))
                config->setIni("msg_key", k2);
            else
            {
                NotifyUI::Show3sNotify(cn("µ¯³öÏûÏ¢¿ì½Ý¼ü×¢²áÊ§°Ü"));
            }
        }

        QString k3 = key3->text();
        if (k3 != config->getIni("lock_key"))
        {
            hot->unRegister(2);
            if (hot->RegisterHotkey(2, k3))
                config->setIni("lock_key", k3);
            else
            {
                NotifyUI::Show3sNotify(cn("ËøÆÁ¿ì½Ý¼ü×¢²áÊ§°Ü"));
            }
        }

        QString k4 = key4->text();
        if (k4 != config->getIni("close_key"))
        {
            hot->unRegister(3);
            if (hot->RegisterHotkey(3, k4))
                config->setIni("close_key", k4);
            else
            {
                NotifyUI::Show3sNotify(cn("¹Ø±ÕÁÄÌì´°¿Ú¿ì½Ý¼ü×¢²áÊ§°Ü"));
            }
        }

        QString k5 = key5->text();
        if (k5 != config->getIni("show_need_replay"))
        {
            hot->unRegister(4);
            if (hot->RegisterHotkey(4, k5))
                config->setIni("show_need_replay", k5);
            else
            {
                NotifyUI::Show3sNotify(cn("ÏÔÊ¾»Ø¸´´°¿Ú¿ì½Ý¼ü×¢²áÊ§°Ü"));
            }
        }

        QString k6 = key6->text();
        if (k6 != config->getIni("hide_active"))
        {
            hot->unRegister(5);
            if (hot->RegisterHotkey(5, k6))
                config->setIni("hide_active", k6);
            else
            {
                NotifyUI::Show3sNotify(cn("Òþ²ØÁÄÌì´°¿Ú¿ì½Ý¼ü×¢²áÊ§°Ü"));
            }
        }

    });
    connect(opt, &OptionsUI::confirm, this, [=] (int tab) {
        if (tab != index)
            return;
        emit opt->apply(tab);
    });
}

OptionKeys::~OptionKeys()
{

}



KeyLineEdit::KeyLineEdit(QWidget *parent)
    : QLineEdit(parent)
{
    setPlaceholderText(QStringLiteral("¼üÈë¿ì½Ý¼ü"));
    setFixedWidth(100);
    setAlignment(Qt::AlignCenter);
    setReadOnly(true);
    QFile file;
    file.setFileName(":res/css/lineedit.css");
    if (file.open(QIODevice::ReadOnly))
    {
        QByteArray ba = file.readAll();
        setStyleSheet(QTextCodec::codecForLocale()->toUnicode(ba));
    }
    file.close();
}

KeyLineEdit::~KeyLineEdit()
{

}

void KeyLineEdit::setDefault(QString keys)
{
    setText(keys);
    defaultKeys = keys;
}


void KeyLineEdit::keyPressEvent(QKeyEvent *e)
{
    QString keys = defaultKeys;
    QString key = e->text().toUpper();
    bool b1 = e->modifiers() & Qt::ShiftModifier;
    bool b2 = e->modifiers() & Qt::ControlModifier;
    bool b3 = e->modifiers() & Qt::AltModifier;

    if (!key.isEmpty())
    {
        if (b1 && b3)
        {
            keys = "Shift + Alt + " + key;
        }
        else if (b2 & b3)
        {
            keys = "Ctrl + Alt + " + key;
        }
        else if (b1 && b2)
        {
        }
        else if (b1)
        {
            keys = "Shift + " + key;
        }
        else if (b3)
        {
            keys = "Alt + " + key;
        }
        else
        {

        }
    }
    setText(keys.toUpper());
    e->ignore();
}


void KeyLineEdit::enterEvent(QEvent *e)
{
    setCursor(QCursor(Qt::ArrowCursor));
    return QLineEdit::enterEvent(e);
}
