#include "lockwidget.h"
#include "../../thwidgets/emoji/emojilabel.h"
#include "../../config/configuredata.h"
#include <QtWidgets>

LockWidget::LockWidget(QWidget *parent) : QWidget(parent)
{
    id = -1;
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);
    setWindowState(Qt::WindowFullScreen);
    setWindowModality(Qt::ApplicationModal);
    setFixedSize(qApp->desktop()->size());
    QVBoxLayout *v = new QVBoxLayout(this);
    v->setContentsMargins(0, 0, 0, 0);
    v->setSpacing(0);

    EmojiLabel *photo = new EmojiLabel(this);
    photo->setMovieable(true);
    photo->setFixedSize(70, 70);
    QLabel *say = new QLabel(this);
    say->setStyleSheet(QStringLiteral("font-family:Î¢ÈíÑÅºÚ;font:16px;color:white;"));

    datetimeLbl = new QLabel(this);
    datetimeLbl->setStyleSheet(QStringLiteral("font-family:Î¢ÈíÑÅºÚ;font:30px;color:white;"));

    QWidget *w = new QWidget(this);
    QHBoxLayout *h = new QHBoxLayout(w);
    h->setContentsMargins(0, 0, 0, 0);
    h->setSpacing(0);
    QPushButton *pb = new QPushButton(w);
    pb->setShortcut(Qt::Key_Return);
    pb->setFixedSize(48, 48);
    QLineEdit *edit = new QLineEdit(w);
    edit->setFixedSize(120, 48);
    edit->setAlignment(Qt::AlignCenter);
    edit->setEchoMode(QLineEdit::Password);
    h->addWidget(edit, 4);
    h->addSpacing(10);
    h->addWidget(pb);

    v->addStretch(4);
    v->addWidget(photo, 0, Qt::AlignCenter);
    v->setSpacing(10);
    v->addWidget(say, 0, Qt::AlignCenter);
    v->addStretch(1);
    v->addWidget(w, 0, Qt::AlignCenter);
    v->addStretch(2);
    v->addWidget(datetimeLbl, 0, Qt::AlignCenter);
    v->addStretch(6);

    QFile file;
    file.setFileName(":res/css/lineedit.css");
    if (file.open(QIODevice::ReadOnly))
    {
        QByteArray ba = file.readAll();
        edit->setStyleSheet(QTextCodec::codecForLocale()->toUnicode(ba));
    }
    file.close();
    file.setFileName(":res/css/button.css");
    if (file.open(QIODevice::ReadOnly))
    {
        QByteArray ba = file.readAll();
        pb->setStyleSheet(QTextCodec::codecForLocale()->toUnicode(ba).
                            arg("arrow").arg("arrow").arg("arrow"));
    }
    file.close();

    // load data
    ConfigureData *config =ConfigureData::getInstance();
    photo->setMoiveRes(config->getIni("photo"));
    say->setText(config->getIni("feel"));
    // connect
    connect(pb, &QPushButton::clicked, this, [=] () {
        QString pw = edit->text();
        if (pw != config->getPassword())
        {
            edit->clear();
            return;
        }
        if (id != -1)
        {
            killTimer(id);
            id = -1;
        }
        this->close();
    });

    id = startTimer(100);

    edit->setFocus();
}

LockWidget::~LockWidget()
{
    qDebug() << "lock deleter";
}



void LockWidget::paintEvent(QPaintEvent *e)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);
    QPainterPath path;
    path.addRect(this->rect());
    p.fillPath(path, QBrush(QColor(50, 50, 50, 150)));
    return QWidget::paintEvent(e);
}


void LockWidget::timerEvent(QTimerEvent *e)
{
    if (e->timerId() == id)
    {
        datetimeLbl->setText(QDateTime::currentDateTime().
                            toString("yyyy-MM-dd hh:mm:ss"));
        this->raise();
        this->activateWindow();
    }
}
