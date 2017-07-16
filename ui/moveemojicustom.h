#ifndef MOVEEMOJICUSTOM_H
#define MOVEEMOJICUSTOM_H

#include <QObject>
#include <QStringList>

class MoveEmojiCustom : public QObject
{
    Q_OBJECT
public:
    MoveEmojiCustom();
    virtual~MoveEmojiCustom();
    static MoveEmojiCustom *GetInstance();
    static void DesInstance();

signals:
    void signalProgress(int value);
    void signalMoveInfo(QString info);

    void signalStartMove(QString res);
    void signalStartMoves(QStringList lst);
private slots:
    void slotStartMove(QString res);
    void slotStartMoves(QStringList lst);
private:
    class PrivData;
    PrivData *mData;
};

#endif // MOVEEMOJICUSTOM_H
