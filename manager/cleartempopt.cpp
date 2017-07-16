#include "cleartempopt.h"
#include "../util/thmethod.h"
#include "../config/configuredata.h"
#include <QDir>
#include <QDebug>

ClearTempOpt::ClearTempOpt()
{
    connect(this, SIGNAL(signalFinished()), this, SLOT(deleteLater()));
}

ClearTempOpt::~ClearTempOpt()
{
    qDebug() << "delete clear thread";
}



void ClearTempOpt::run()
{
    // 清空缓存
    QString path = GetWorkPath();
    path = path + "/temp";
    QDir dir(path);
    QFileInfoList lst = dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot);
    if (lst.isEmpty())
    {
        emit signalProgressinfo(cns("没有文件"));
        emit signalProgress(100);
    }
    for (int i = 0; i < lst.size(); i++)
    {
        QFileInfo info = lst.at(i);
        QString name = info.absoluteFilePath();
        if (info.isFile())
        {
            QFile file(name);
            if (file.exists())
            {
                if (file.remove())
                {
                    emit signalProgressinfo(name + cns(" 移除成功"));
                }
                else
                {
                    emit signalProgressinfo(name + cns(" 移除失败"));
                }
            }
            else
            {
                emit signalProgressinfo(name + cns(" 不存在"));
            }
        }
        float f1 = i + 1;
        float f2 = lst.size();
        int value = f1 / f2 * 100;
        emit signalProgress(value);
    }
    emit signalProgress(101);
    emit signalFinished();
}
