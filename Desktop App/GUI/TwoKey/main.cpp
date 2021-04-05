#include "twokey.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TwoKey w;
    w.show();
    return a.exec();
}
