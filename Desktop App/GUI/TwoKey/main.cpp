#include "login.h"
#include "createaccount.h"
#include "manager.h"
#include "addaccount.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    manager start_widget;
    start_widget.show();
    return a.exec();
}
