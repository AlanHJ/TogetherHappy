#include "emojilabel.h"
#include <QMovie>
#include <QtConcurrent/QtConcurrent>
#include "../../sql/sql.h"
#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>

EmojiLabel::EmojiLabel(QWidget *parent) : QLabel(parent)
{
    setAlignment(Qt::AlignCenter);
    isPressed = false;
    isMovie = false;
    isChecked = false;
    isCheckable = false;
    setFocusPolicy(Qt::NoFocus);
}

EmojiLabel::EmojiLabel(QString file, QSize size, QWidget *parent)
    : QLabel(parent)
{
    isPressed = false;
    isMovie = false;
    isChecked = false;
    isCheckable = false;
    setFocusPolicy(Qt::NoFocus);
    setAlignment(Qt::AlignCenter);
    QMovie *movie = new QMovie(this);
    movie->setFileName(file);
    if (size == QSize(0, 0))
        setScaledContents(true);
    else
        movie->setScaledSize(size);
    setMovie(movie);
    setContentsMargins(0, 0, 0, 0);
    // ³õÊ¼»¯ÏÔÊ¾
    movie->start();
    movie->stop();
}

EmojiLabel::~EmojiLabel()
{
//    qDebug() << "emoji delete!!!";
}

QString EmojiLabel::ImagePath()
{
    if (!ed.getPath().isEmpty())
    {
        return ed.getPath();
    }
    else
    {
        QMovie *m= movie();
        if (!m)
            return "";
        return m->fileName();
    }
}

void EmojiLabel::setMoiveRes(QString path, QSize size)
{
    QMovie *m = movie();
    if (nullptr != m)
        delete m;
    QMovie *movie = new QMovie(this);
    movie->setFileName(path);
    QRect rect1(QPoint(0, 0), this->size());
    QRect rect2(QPoint(0, 0), QImage(path).size());
    bool b = rect1.contains(rect2);
    if (size == QSize(0, 0))
        setScaledContents(!b);
    else
        movie->setScaledSize(size);
    setMovie(movie);
    movie->start();
    if (!isMovie)
        movie->stop();
}

void EmojiLabel::setData(EmoticonData data)
{
    ed = data;
    if (!ed.getThumb().isEmpty())
    {
        setMoiveRes(ed.getThumb());
    }
    else
    {
        setMoiveRes(ed.getPath());
    }
}

EmoticonData EmojiLabel::getData()
{
    return ed;
}

void EmojiLabel::setMovieable(bool b)
{
    isMovie = b;
    QMovie *m = movie();
    if (!m)
        return;
    if (isMovie)
        m->start();
    else
        m->stop();
}

void EmojiLabel::setCheckable(bool b)
{
    isCheckable = b;
}

void EmojiLabel::setChecked(bool b)
{
    if (!isCheckable)
        return;
    isChecked = b;
    update();
    emit toggled(ed, isChecked);
}

bool EmojiLabel::getChecked()
{
    return isChecked;
}

void EmojiLabel::enterEvent(QEvent *event)
{
    emit enterEmoji();
    QMovie *m = movie();
    if (!m)
        return;
    m->start();
    return QLabel::enterEvent(event);
}

void EmojiLabel::leaveEvent(QEvent *event)
{
    emit leaveEmoji();
    QMovie *m = movie();
    if (isMovie)
        return;
    if (!m)
        return;
    m->stop();
    return QLabel::leaveEvent(event);
}


void EmojiLabel::mousePressEvent(QMouseEvent *)
{
    isPressed = true;
}

void EmojiLabel::mouseReleaseEvent(QMouseEvent *e)
{
    if (isPressed && e->button() == Qt::LeftButton)
    {
        isPressed = false;
        emit clicked(ed);
        if (isCheckable)
        {
            isChecked = !isChecked;
            update();
            emit toggled(ed, isChecked);
        }
    }
}


void EmojiLabel::paintEvent(QPaintEvent *e)
{
    QLabel::paintEvent(e);
    if (!isCheckable)
        return;
    if (!isChecked)
        return;
    int w = this->width();
    int h = this->height();
    QImage image(":res/ui3/checked2.png");
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);
    QPainterPath path;
    path.addRoundedRect(QRect(0, 0, w, h), 3, 3);
    QRectF target = image.rect();
    target.moveCenter(this->rect().center());
    QRectF source = image.rect();
    p.fillPath(path, QBrush(QColor(50, 50, 50, 50)));
    p.drawImage(target, image, source);
}
