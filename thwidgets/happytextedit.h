#ifndef HAPPYTEXTEDIT_H
#define HAPPYTEXTEDIT_H

#include <QTextEdit>
#include "../ui/thwidgetbase.h"


class ShowImage : public THWidgetBase
{
    Q_OBJECT
public:
    ShowImage(QString path, QWidget *parent = 0);
    ~ShowImage();

    // QWidget interface
protected:
    void resizeEvent(QResizeEvent *e);
};

// gif movie support

class HappyTextEdit : public QTextEdit
{
    Q_OBJECT
public:
    HappyTextEdit(QWidget *parent = 0);
    ~HappyTextEdit();
    void setReadOnly1(bool b);
    bool isMaxVertical();

public slots:
    void scrollMax();
    void clearContent();
    void appendHtml(QString &html);
    void insertHtml1(QString &html);

    // change format
    void txtBold(bool b);
    void txtUnderLine(bool b);
    void txtItalic(bool b);
    void txtFamily(QString f);
    void txtSize(float f);
    void txtColor(QColor color);
    void txtAlign(Qt::Alignment a);

signals:
    void returnPress();
    void scrollValueMax();
    void signalGetPath(QString path);
    void addEmotion(QString path, QString tab);

    // format changed
    void fontChanged(QFont f);
    void colorChanged(QColor color);
    void alignmentChanged(Qt::Alignment a);

    // QWidget interface
protected:
    void keyPressEvent(QKeyEvent *event) override;
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void insertFromMimeData(const QMimeData *source) override;
    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseDoubleClickEvent(QMouseEvent *e);
    void contextMenuEvent(QContextMenuEvent *e);

private:
    void parserHtml(QString html);
    void mergeFormatOnWordOrSelection(const QTextCharFormat &format);
    bool findImageByPos(QPoint p, QString &path);

    QHash<QString, QMovie*> movs;
    QStringList content;
    QTextCharFormat curFmt;

signals:
    void parserHtmlFinished(QStringList list);

private slots:
    void buildMovies(QStringList list);


    // QAbstractScrollArea interface
protected:
    void scrollContentsBy(int dx, int dy);
};

#endif // HAPPYTEXTEDIT_H
