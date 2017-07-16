#ifndef LOGOUT_H
#define LOGOUT_H

#include <QFile>

class Logout
{
private:
    Logout();
    virtual~Logout();
    static Logout *log;
    QFile file;
    bool isOk;
public:
    static Logout *GetInstance();
    static void Destruct();
    void write(QString type, QString msg);
};

#endif // LOGOUT_H
