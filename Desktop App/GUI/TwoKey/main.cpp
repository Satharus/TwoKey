#include "login.h"
#include "createaccount.h"
#include "manager.h"
#include "addaccount.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    login w;
    w.show();
    createaccount e;
    e.show();
    manager r;
    r.show();
    addaccount t;
    t.show();
    return a.exec();
}
