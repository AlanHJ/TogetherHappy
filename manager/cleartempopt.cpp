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
    // ��ջ���
    QString path = GetWorkPath();
    path = path + "/temp";
    QDir dir(path);
    QFileInfoList lst = dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot);
    if (lst.isEmpty())
    {
        emit signalProgressinfo(cns("û���ļ�"));
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
                    emit signalProgressinfo(name + cns(" �Ƴ��ɹ�"));
                }
                else
                {
                    emit signalProgressinfo(name + cns(" �Ƴ�ʧ��"));
                }
            }
            else
            {
                emit signalProgressinfo(name + cns(" ������"));
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
