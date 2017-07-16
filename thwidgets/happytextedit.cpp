#include "happytextedit.h"
#include "../util/thmethod.h"
#include "../config/configuredata.h"
#include "../ui/happytitlebar.h"
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QMimeData>
#include <QKeyEvent>
#include <QDragEnterEvent>
#include <QImageReader>
#include <QMovie>
#include <QtConcurrent/QtConcurrent>
#include <QDesktopServices>
#include <QPalette>
#include <QTextBlock>
#include <QHBoxLayout>
#include <QScrollBar>
#include <QFileDialog>
#include <QAbstractTextDocumentLayout>
#include <QTextDocumentFragment>
#include <QTextFormat>
#include "../ui/ui3/selfmenu.h"
#include "emoji/emojilabel.h"
#include "../ui/ui3/combox.h"
#include "../sql/sql.h"
#include "../ui/ui3/msgbox.h"
#include "../data/db/emoticongroupdata.h"
#include "../ui/notifyui.h"
//#define NOUSED

HappyTextEdit::HappyTextEdit(QWidget *parent)
    : QTextEdit(parent)
{
//    setContextMenuPolicy(Qt::NoContextMenu);
    connect(this, SIGNAL(parserHtmlFinished(QStringList)),
            this, SLOT(buildMovies(QStringList)));

    connect(this, &QTextEdit::cursorPositionChanged, [=] () {
        if (isReadOnly())
            return;
        emit alignmentChanged(alignment());
        if (textCursor().position() == 0)
                mergeFormatOnWordOrSelection(curFmt);
    });
    connect(this, &QTextEdit::currentCharFormatChanged, [=] () {
        if (isReadOnly())
            return;
        mergeFormatOnWordOrSelection(curFmt);
        emit fontChanged(currentCharFormat().font());
        emit colorChanged(currentCharFormat().foreground().color());
    });
    QScrollBar *vbar = verticalScrollBar();
    connect(vbar, &QScrollBar::valueChanged, this, [=] (int value) {
        if (value == vbar->maximum())
            emit scrollValueMax();
    });

    QTimer::singleShot(10, this, [=] () {
        emit fontChanged(font());
        emit colorChanged(textColor());
        emit alignmentChanged(alignment());
    });

    // set style
    QFile file(":/res/css/edit.css");
    if (file.open(QIODevice::ReadOnly))
    {
        setStyleSheet(file.readAll());
    }
    file.close();
    QPalette plt = palette();
    plt.setColor(QPalette::Text, Qt::white);
    setPalette(plt);
}

HappyTextEdit::~HappyTextEdit()
{
    // �������������ɱ����Զ�����
    clearContent();
}

void HappyTextEdit::setReadOnly1(bool b)
{
//    setMouseTracking(b);
    setReadOnly(b);
    if (b)
    {
        viewport()->setCursor(QCursor(Qt::ArrowCursor));
    }
    else
    {
        viewport()->setCursor(QCursor(Qt::IBeamCursor));
    }
}

bool HappyTextEdit::isMaxVertical()
{
    QScrollBar *bar = verticalScrollBar();
    int value = bar->value();
    int max = bar->maximum();
    if (value != max)
        return false;
    else
        return true;
}

void HappyTextEdit::scrollMax()
{
    QScrollBar *bar = verticalScrollBar();
    bar->setValue(bar->maximum());
}

void HappyTextEdit::clearContent()
{
    // delete movies
    auto it = movs.begin();
    while (it != movs.end())
    {
        QMovie *m = it.value();
        m->stop();
        disconnect(m, 0, 0, 0);
        delete m;
        m = nullptr;
        ++it;
    }
    movs.clear();
    this->clear();
    content.clear();
}

void HappyTextEdit::appendHtml(QString &html)
{
    append(html);
    //need viewport resource parser
    QtConcurrent::run(this, &HappyTextEdit::parserHtml, html);
}

void HappyTextEdit::insertHtml1(QString &html)
{
    insertHtml(html);
    // ����һ���߳������������е�image��Դ
    QtConcurrent::run(this, &HappyTextEdit::parserHtml, html);
}

void HappyTextEdit::txtBold(bool b)
{
    QTextCharFormat fmt;
    fmt.setFontWeight(b ? QFont::Bold : QFont::Normal);
    curFmt.setFontWeight(fmt.fontWeight());
    mergeFormatOnWordOrSelection(fmt);
}

void HappyTextEdit::txtUnderLine(bool b)
{
    QTextCharFormat fmt;
    fmt.setFontUnderline(b);
    curFmt.setFontUnderline(b);
    mergeFormatOnWordOrSelection(fmt);
}

void HappyTextEdit::txtItalic(bool b)
{
    QTextCharFormat fmt;
    fmt.setFontItalic(b);
    curFmt.setFontItalic(b);
    mergeFormatOnWordOrSelection(fmt);
}

void HappyTextEdit::txtFamily(QString f)
{
    QTextCharFormat fmt;
    fmt.setFontFamily(f);
    curFmt.setFontFamily(f);
    mergeFormatOnWordOrSelection(fmt);
}

void HappyTextEdit::txtSize(float f)
{
    qreal pointSize = f;
    if (f > 0)
    {
        QTextCharFormat fmt;
        fmt.setFontPointSize(pointSize);
        curFmt.setFontPointSize(pointSize);
        mergeFormatOnWordOrSelection(fmt);
    }
}

void HappyTextEdit::txtColor(QColor color)
{
    if (!color.isValid())
        return;
    QTextCharFormat fmt;
    fmt.setForeground(color);
    curFmt.setForeground(color);
    mergeFormatOnWordOrSelection(fmt);
}

void HappyTextEdit::txtAlign(Qt::Alignment a)
{
    setAlignment(a);
}


void HappyTextEdit::keyPressEvent(QKeyEvent *event)
{
    if (isReadOnly())
        return QTextEdit::keyPressEvent(event);
    QString inikey = ConfigureData::getInstance()->getIni("send_key");
    int id = 0;
    if (inikey.compare("Ctrl + Enter", Qt::CaseInsensitive) == 0)
    {
        id = 1;
    }
    int key = event->key();
    if (key == Qt::Key_Return && id == 0)
    {
        emit returnPress();
        clearContent();
        return;
    }
    else if ((key == Qt::Key_Return && event->modifiers() & Qt::ControlModifier) && id == 1)
    {
        emit returnPress();
        clearContent();
        return;
    }
    QTextEdit::keyPressEvent(event);
}


void HappyTextEdit::dragEnterEvent(QDragEnterEvent *event)
{
    QTextEdit::dragEnterEvent(event);
}

void HappyTextEdit::dropEvent(QDropEvent *event)
{
    QTextEdit::dropEvent(event);
}

void HappyTextEdit::insertFromMimeData(const QMimeData *source)
{
    if (source->hasImage())
    {
        QImage image = qvariant_cast<QImage>(source->imageData());

        QString path = GetWorkPath();
        path = path + "/temp";
        QDir dir(".");
        QFileInfo info(path);
        if (!info.exists())
        {
            dir.mkpath(info.absoluteFilePath());
        }
        QString name = info.absoluteFilePath() + "/" + getUuid() + "clipper.png";
        image.save(name);
        this->insertHtml1(ImagePathToFileHtml(name));
        return;
    }

    if (source->hasHtml())
    {
        // �ҵ�HTML�����е�ͼƬ��Դ
        QtConcurrent::run(this, &HappyTextEdit::parserHtml, source->html());
    }

    if (source->hasUrls())
    {
        QList<QUrl> lst = source->urls();
        auto it = lst.begin();
        while (it != lst.end())
        {
            QUrl url = *it;
            QString path = url.path();
            path = path.right(path.length() - 1);
            if (QFileInfo(path).isFile())
            {
                QImageReader reader(path);
                if (reader.canRead())
                {
                    // image file
                    this->insertHtml1(ImagePathToHtml(path));
                }
                else
                {
                    // other file
                    emit signalGetPath(path);
                }
            }
            else
            {
                // Ŀ¼
//                QString temp = path.right(path.length() - 1);
                emit signalGetPath(path);
            }
            ++it;
        }
        return;
    }
    QTextEdit::insertFromMimeData(source);
}

void HappyTextEdit::mouseMoveEvent(QMouseEvent *e)
{
    return QTextEdit::mouseMoveEvent(e);
    QPoint p = e->pos();
    QString str = anchorAt(p);
    bool b = QUrl(str).isValid();
    if (isReadOnly())
    {
        if (b)
            viewport()->setCursor(QCursor(Qt::PointingHandCursor));
        else
            viewport()->setCursor(QCursor(Qt::ArrowCursor));
    }
    else
        viewport()->setCursor(QCursor(Qt::IBeamCursor));

}

void HappyTextEdit::mousePressEvent(QMouseEvent *e)
{
    return QTextEdit::mousePressEvent(e);
    if (viewport()->cursor().shape() == Qt::PointingHandCursor)
    {
        QString str = anchorAt(e->pos());
        if (str == HAPPYURL)
        {
            // load more
            ;
        }
        else
            QDesktopServices::openUrl(QUrl(str));

    }
}

void HappyTextEdit::parserHtml(QString html)
{
    emit parserHtmlFinished(ParserHtmlImgSrc(html));
}

void HappyTextEdit::mergeFormatOnWordOrSelection(const QTextCharFormat &format)
{
    QTextCursor cursor = textCursor();
    if (!cursor.hasSelection())
        cursor.select(QTextCursor::WordUnderCursor);
    cursor.mergeCharFormat(format);
    mergeCurrentCharFormat(format);
}

bool HappyTextEdit::findImageByPos(QPoint p, QString &path)
{
    QPoint pos = QPoint(p.x() + horizontalScrollBar()->value(), p.y() + verticalScrollBar()->value());
    int cursorPos = document()->documentLayout()->hitTest(pos, Qt::ExactHit);
    if (cursorPos == -1)
        return false;
    QTextDocument *doc = document();
    QTextBlock block = doc->findBlock(cursorPos);
    QTextBlock::iterator it = block.begin();
    while (it != block.end())
    {
        QTextFragment tf = it.fragment();
        QTextCharFormat tcf = tf.charFormat();
        int fpos = tf.position();
        if (tcf.isImageFormat() && fpos == cursorPos)
        {
            QString p = tcf.toImageFormat().name();
            if (p.contains("file:///"))
                p = p.remove("file:///");
            path = p;
            return true;
        }
        ++it;
    }
    return false;
}

void HappyTextEdit::buildMovies(QStringList list)
{
    if (document()->isEmpty())
    {
        clearContent();
    }
    auto it = list.begin();
    while (it != list.end())
    {
        QString path = *it;
        if (!movs.contains(path))
        {
            QImageReader reader(path);
            if (reader.supportsAnimation())
            {
                QMovie *m = new QMovie(this);
                m->setFileName(path);
                m->setCacheMode(QMovie::CacheNone);
                connect(m, &QMovie::frameChanged, this, [=] () {
                    document()->addResource(QTextDocument::ImageResource,
                                            QUrl(m->fileName(), QUrl::StrictMode), m->currentPixmap());
                    setLineWrapColumnOrWidth(lineWrapColumnOrWidth());
                });
                m->start();
                movs.insert(path, m);
            }
        }
        ++it;
    }
}

void HappyTextEdit::scrollContentsBy(int dx, int dy)
{
//    QTextCharFormat tcf = currentCharFormat();
//    if (tcf.isImageFormat())
//    {
//        qDebug() << tcf.toImageFormat().name();
//    }
    // find view port need to gif
//    QTextBlock block = document()->begin();
//    int viewPortH = minimumSizeHint().height();
//    int currentPos = verticalScrollBar()->value();
//    while (block.isValid())
//    {
//        int pos = block.position();
//        if (pos >= (currentPos - 10) && pos <= (currentPos + viewPortH + 10))
//        {
//            auto it = block.begin();
//            while (it != block.end())
//            {
//                QTextFragment fmt = it.fragment();
//                if (fmt.isValid())
//                {
//                    qDebug() <<fmt.text();
//                }
//                ++it;
//            }
//        }
//        block = block.next();
//    }


    return QTextEdit::scrollContentsBy(dx, dy);
}

void HappyTextEdit::mouseDoubleClickEvent(QMouseEvent *e)
{
    if (e->button() != Qt::LeftButton)
        return QTextEdit::mouseDoubleClickEvent(e);
    QPoint pos = e->pos();
    QString path;
    if (findImageByPos(pos, path))
    {
        ShowImage *image = new ShowImage(path);
        image->show();
        image->activateWindow();
        movecenter(image);
    }
    e->accept();
//        pos = QPoint(pos.x() + horizontalScrollBar()->value(), pos.y() + verticalScrollBar()->value());
//        int cursorPos = document()->documentLayout()->hitTest(pos, Qt::ExactHit);
//        if (cursorPos == -1)
//            return;
//        QTextDocument *doc = document();
//        QTextBlock block = doc->findBlock(cursorPos);
//        QTextBlock::iterator it = block.begin();
//        while (it != block.end())
//        {
//            QTextFragment tf = it.fragment();
//            QTextCharFormat tcf = tf.charFormat();
//            int fpos = tf.position();
//            if (tcf.isImageFormat() && fpos == cursorPos)
//            {
//                QString path = tcf.toImageFormat().name();
//                if (path.contains("file:///"))
//                    path = path.remove("file:///");
//                EmojiLabel *lab = new EmojiLabel;
//                lab->setMoiveRes(path);
//                lab->setMovieable(true);
//                lab->show();
//                lab->activateWindow();
//                break;
//            }
//            ++it;
//        }


// method 2;
//        QTextBlock &currentBlock = doc->begin();
//        while( true)
//        {
//            // ���޸�chatformatʱ��ı䵱ǰBlock��fragment
//            // ����++it�Ĵ�������std::map�е�erase����
//            for (it = currentBlock.begin(); !(it.atEnd()); )
//            {
//                QTextFragment currentFragment = it.fragment();
//                QTextImageFormat newImageFormat = currentFragment.charFormat().toImageFormat();

//                if (newImageFormat.isValid()) {
//                    // �жϳ����fragmentΪimage
//                    int curpos = it.fragment().position();
//                    if (cursorPos == curpos)
//                    {
//                        QString p = newImageFormat.name();
//                        EmojiLabel *lab = new EmojiLabel;
//                        lab->setMoiveRes(p);
//                        lab->setMovieable(true);
//                        lab->show();
//                        lab->activateWindow();
//                    }
//                }
//                ++it;
//            }

//            currentBlock = currentBlock.next();
//            if(!currentBlock.isValid())
//                break;
//        }


//    QPoint pos = e->pos();
//    QString str = anchorAt(pos);
//    int cursorPos = document()->documentLayout()->hitTest(pos, Qt::ExactHit);
////    if (cursorPos == -1)
////        return;;
//    QTextBlock block = document()->firstBlock();
//    while (block.isValid()) {
////        QRectF blockBr = document()->documentLayout()->blockBoundingRect(block);
////        if (blockBr.contains(pos)) {
//            auto it = block.begin();
//            while (it != block.end())
//            {
//                QTextCursor tcur(block);
//                QTextCharFormat charf = tcur.charFormat();
//                if (charf.isImageFormat())
//                {
//                    QString name = charf.toImageFormat().name();
//                    EmojiLabel *lab = new EmojiLabel;
//                    lab->setWindowTitle(QString::number(tcur.position()));
//                    lab->setMoiveRes(name);
//                    lab->show();
//                    lab->activateWindow();
//                }

//                ++it;
//            }
////            QTextLayout *layout = block.layout();
////            int relativeCursorPos = cursorPos - block.position();
////            const int preeditLength = layout ? layout->preeditAreaText().length() : 0;
////            if (preeditLength > 0 && relativeCursorPos > layout->preeditAreaPosition())
////                cursorPos -= qMin(cursorPos - layout->preeditAreaPosition(), preeditLength);
////            break;
////        }
//        block = block.next();
//    }
}


void HappyTextEdit::contextMenuEvent(QContextMenuEvent *e)
{
    if (!isReadOnly())
        return;
    QPoint p = e->pos();
    QString path;
    if (findImageByPos(p, path))
    {
        SelfMenu menu;
        SelfMenu *cmenu = new SelfMenu(cn("��ӱ��鵽..."), &menu);
        connect(menu.addAction(cn("���Ϊ")), &QAction::triggered,
                this, [=] () {
            QString filter = cn("ͼƬ��Դ(*.jpg *.gif *.png*.bmp);;ȫ���ļ�(*.*)");
            QString directory =
                    QFileDialog::getSaveFileName(this,
                                                  cns("ѡ�����ΪĿ¼"),
                                                  "D:/" + QFileInfo(path).fileName(),
                                                  filter);
            if (directory.isEmpty())
            {
                NotifyUI::ShowNotify(cn("������Ч��Ŀ¼"));
                return;
            }
            QFile file(path);
            if (file.copy(directory))
            {
                NotifyUI::ShowNotify(cn("���Ϊ�ɹ�"));
            }
            else
            {
                NotifyUI::ShowNotify(cn("���Ϊʧ��"));
            }
        });
        menu.addMenu(cmenu);
        QActionGroup *ag = new QActionGroup(&menu);
        QString er;
        SqlDB *db = Sql::instance()->open(er);
        if (!db)
        {
//            MsgBox::ShowMsgBox(cn("ʧ��"), cn("��ѯ������Ϣ����\n") + er, cn("ȷ��"));
            NotifyUI::Show3sNotify(cn("ʧ��,��ѯ������Ϣ����\n") + er);
            return;
        }
        int rlt = 0;
        QString sql = "SELECT * FROM emoticon_group";
        QList<QObject *> objs = db->executeQuery(sql, EmoticonGroupData().metaObject(),
                                                 rlt, er);
        db->close();
        if (objs.isEmpty())
        {
//            MsgBox::ShowMsgBox(cn("ʧ��"), cn("û���κη���\n���Ƚ�������"), cn("ȷ��"));
            NotifyUI::Show3sNotify(cn("ʧ��,û���κη���\n���Ƚ�������") + er);
            return;
        }
        auto it  = objs.begin();
        while (it != objs.end())
        {
            QObject *obj = *it;
            EmoticonGroupData *egd = qobject_cast<EmoticonGroupData *>(obj);
            if (egd && egd->getEditable())
            {
                QString tab = egd->getGroup();
                ag->addAction(cmenu->addAction(tab));
            }
            delete obj;
            obj = nullptr;
            ++it;
        }
        connect(ag, &QActionGroup::triggered,
                this, [=] (QAction *a) {
                if (!a)
                    return;
                emit addEmotion(path, a->text());
        });
        menu.exec(QCursor::pos());
    }
    e->ignore();
}


ShowImage::ShowImage(QString path, QWidget *parent)
    : THWidgetBase(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
    bar->setBarButtons(HappyTitleBar::MinButtonHint);
    bar->setBarButtons(HappyTitleBar::MaxButtonHint);
    bar->setBarIcon(":res/happy.png");
    bar->setBarContent(cn("ͼƬ�鿴"));
    bar->setExtraButtonVisible(false);
    setSizeEnable(true, QSize(100, 100));
    connect(bar, &HappyTitleBar::signalClose, this, [=] () {
        close();
    });
    QHBoxLayout *h = new QHBoxLayout(this);
    h->setContentsMargins(10, 30, 10, 10);
    EmojiLabel *image = new EmojiLabel(this);
    image->setMoiveRes(path);
    image->setMovieable(true);
    h->addWidget(image);
    setMouseTracking(true);
    setMinimumSize(150, 150);
    // load data
    ConfigureData *conf = ConfigureData::getInstance();
    updateColor(conf->getColorIni("color1"), conf->getColorIni("color2"));
}

ShowImage::~ShowImage()
{
    qDebug() << "show destruct";
}


void ShowImage::resizeEvent(QResizeEvent *e)
{
    THWidgetBase::resizeEvent(e);
    setTitleBarWidth(this->width());
}
