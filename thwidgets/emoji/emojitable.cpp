#include "emojitable.h"
#include "emojilabel.h"
#include "../../config/configuredata.h"
#include "../../util/thmethod.h"
#include <QHeaderView>
#include <QDir>
#include <QMenu>
#include <QPoint>
#include <QBitmap>
#include <QAction>
#include <QFileInfo>
#include <QMouseEvent>
#include <QHBoxLayout>
#include <QScrollBar>

EmojiTable::EmojiTable(QWidget *parent)
    : QTableWidget(parent)
{

    setWindowFlags(Qt::Tool);

    // 初始化Table设置
    setDragEnabled(true);
    setGridStyle(Qt::NoPen);
    setFocusPolicy(Qt::NoFocus);
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    horizontalHeader()->setVisible(false);
    verticalHeader()->setVisible(false);

    QFile file;
    file.setFileName(":res/css/table.css");
    if (file.open(QIODevice::ReadOnly))
    {
        QByteArray ba = file.readAll();
        setStyleSheet(QTextCodec::codecForLocale()->toUnicode(ba));
    }
    file.close();
}

EmojiTable::~EmojiTable()
{

}

void EmojiTable::insertEmoji(QList<EmoticonData> datas, bool ischeckable, QSize size)
{
    // 添加表情

    // 获取当前已经添加表情的个数
    selectAll();

    int TableCounts = rowCount() * columnCount();
    int ItemCounts = this->selectedItems().size();
    clearSelection();
    // 是否需要增加行列
    int newTables = datas.size() - (TableCounts - ItemCounts);
    if (newTables > 0)
    {
        // 按需增加
        int c1 = newTables / DefaultColumn;
        int c2 = newTables % DefaultColumn;
        if (c1 > 0 && c2 > 0)
        {
            c1++;
        }
        if (c1 == 0)
            c1 += 1;
        QScrollBar *vbar = verticalScrollBar();
//        qDebug() << "row counts" << rowCount() << "  vbar max value:" << vbar->maximum();
        setRowCount(rowCount() + c1);
        vbar->setMaximum(rowCount());
        vbar->setSliderPosition(rowCount());
//        qDebug() << "row counts" << rowCount() << "  vbar max value:" << vbar->maximum();
    }

    for (int i = 0; i < datas.size(); i++)
    {
        EmoticonData ed = datas.at(i);
        // 计算当前应该添加到哪一位置
        int row = ItemCounts / columnCount();
        int column = ItemCounts % columnCount();
        QTableWidgetItem *item = new QTableWidgetItem();
        item->setBackground(QBrush(Qt::transparent));
        setItem(row, column, item);

        QWidget *w = new QWidget(this);
        w->setFocusPolicy(Qt::NoFocus);
        QHBoxLayout *lay = new QHBoxLayout(w);
        lay->setContentsMargins(0, 0, 0, 0);
        lay->setMargin(0);
        EmojiLabel *emoji = new EmojiLabel(this);
//        emoji->setContentsMargins(3, 3, 3, 3);
        emoji->setCheckable(ischeckable);
        emoji->setFixedSize(size);
        emoji->setData(ed);
        lay->addWidget(emoji);
        lay->setAlignment(emoji, Qt::AlignCenter);

        connect(emoji, &EmojiLabel::enterEmoji, this, [=] () {
            emoji->setContentsMargins(3, 3, 3, 3);
        });
        connect(emoji, &EmojiLabel::leaveEmoji, this, [=] () {
            emoji->setContentsMargins(0, 0, 0, 0);
        });

        connect(emoji, &EmojiLabel::clicked, this, [=] (EmoticonData ed) {
            emit emojiClicked(ed);
        });

        if (ischeckable)
        {
            connect(emoji, &EmojiLabel::toggled, this, [=] (EmoticonData ed, bool b) {
                emit emojiToggled(ed, b);
            });
        }

        connect(emoji, &EmojiLabel::enterEmoji, this, [=] () {
            emit enterEmoji(emoji->ImagePath());
        });
        connect(emoji, SIGNAL(leaveEmoji()), this, SIGNAL(leaveEmoji()));
        setCellWidget(row, column, w);
        ItemCounts++;
    }
//    this->scrollToBottom();
}

void EmojiTable::updateEmoji(EmoticonData ed, bool ischeckable, QSize size)
{
    QList<EmoticonData> lst;
    lst.append(ed);
    insertEmoji(lst, ischeckable, size);
}

void EmojiTable::setCheckedAll(bool b)
{
    selectAll();
    QList<QTableWidgetItem *> list = selectedItems();
    selectAll();
    auto it = list.begin();
    while (it != list.end())
    {
        QTableWidgetItem *item = *it;
        QWidget *w = cellWidget(item->row(), item->column());
        EmojiLabel *emoji = qobject_cast<EmojiLabel *>(w->children().at(1));
        if (emoji)
        {
            emoji->setChecked(b);
        }
        ++it;
    }
}

void EmojiTable::initEmojiTable(QSize size)
{
    // 初始化行列
    setRowCount(DefaultRow);
    setColumnCount(DefaultColumn);
    horizontalHeader()->setDefaultSectionSize(size.width());
    verticalHeader()->setDefaultSectionSize(size.height());
}


bool EmojiTable::eventFilter(QObject *obj, QEvent *e)
{
    return QTableWidget::eventFilter(obj, e);
}



void EmojiTable::mousePressEvent(QMouseEvent *e)
{
    return QTableWidget::mousePressEvent(e);
}


void EmojiTable::leaveEvent(QEvent *e)
{
    return QTableWidget::leaveEvent(e);
}
