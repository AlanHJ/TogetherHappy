#include "editlabel.h"
#include "../../util/thmethod.h"
#include <QtWidgets>

EditLabel::EditLabel(QWidget *parent) : QLineEdit(parent)
{
    css = cns("QLineEdit{font-family:Î¢ÈíÑÅºÚ;font-size:12px;"
              "color:%1;"
              "background:%2;"
              "border-radius: 3px;"
              "border-width:1px;"
              "border-style:solid;"
              "border-color: transparent;"
              "selection-background-color:rgba(27, 87, 97, 200);}"
              "QLineEdit:hover{border-color: white;}");
    this->setStyleSheet(css.arg("white").arg("transparent"));
    connect(this, SIGNAL(editingFinished()),
            this, SLOT(slotEditFinished()));
    isClicked = false;
}

EditLabel::~EditLabel()
{

}

void EditLabel::slotEditFinished()
{
    this->clearFocus();
    this->setStyleSheet(css.arg("white").arg("transparent"));
    isClicked = false;
    setCursor(QCursor(Qt::ArrowCursor));
}

void EditLabel::init()
{

}

void EditLabel::mousePressEvent(QMouseEvent *e)
{
    this->setCursor(QCursor(Qt::IBeamCursor));
    this->setStyleSheet(css.arg("white").arg("rgba(230, 230, 230, 50)"));
    if (!isClicked)
    {
        QTimer::singleShot(100, this, SLOT(selectAll()));
//        this->selectAll();
//        qDebug() << "select all!!!";
    }
    isClicked = true;
    QLineEdit::mousePressEvent(e);
}

void EditLabel::enterEvent(QEvent *e)
{
    setCursor(QCursor(Qt::ArrowCursor));
    return QLineEdit::enterEvent(e);
}

void EditLabel::leaveEvent(QEvent *e)
{
    setCursor(QCursor(Qt::ArrowCursor));
    return QLineEdit::leaveEvent(e);
}

void EditLabel::keyPressEvent(QKeyEvent *e)
{
    int key = e->key();
    if (key == Qt::Key_Enter || key == Qt::Key_Return)
    {
        this->parentWidget()->setFocus();
    }
    return QLineEdit::keyPressEvent(e);
}


SearchLabel::SearchLabel(QWidget *parent)
    : QLineEdit(parent)
{
    css = cns("QLineEdit{font-family:Î¢ÈíÑÅºÚ;font-size:13px;"
              "color:white;"
              "background:transparent;"
              "border-radius: 3px;"
              "border-width:1px;"
              "border-style:solid;"
              "border-color: transparent;"
              "selection-background-color:rgba(27, 87, 97, 200);}"
              "QLineEdit:hover{border-color: transparent;}");
    this->setStyleSheet(css);
    connect(this, SIGNAL(editingFinished()),
            this, SLOT(slotEditFinished()));
    isClicked = false;
}

SearchLabel::~SearchLabel()
{

}

void SearchLabel::slotEditFinished()
{

}

void SearchLabel::mousePressEvent(QMouseEvent *e)
{
    this->setCursor(QCursor(Qt::IBeamCursor));
    if (!isClicked)
    {
        QTimer::singleShot(100, this, SLOT(selectAll()));
    }
    isClicked = true;
    QLineEdit::mousePressEvent(e);
}

void SearchLabel::enterEvent(QEvent *e)
{
    setCursor(QCursor(Qt::ArrowCursor));
    return QLineEdit::enterEvent(e);
}

void SearchLabel::leaveEvent(QEvent *e)
{
    setCursor(QCursor(Qt::ArrowCursor));
    return QLineEdit::leaveEvent(e);
}

void SearchLabel::keyPressEvent(QKeyEvent *e)
{
    int key = e->key();
    if (key == Qt::Key_Enter || key == Qt::Key_Return)
    {
        this->parentWidget()->setFocus();
        emit enterPress();
    }
    return QLineEdit::keyPressEvent(e);
}

void SearchLabel::init()
{

}
