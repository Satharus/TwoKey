#include "usb_event_handler.h"
#include "mainwindow.h"

#include <QAbstractNativeEventFilter>
#include <QAbstractEventDispatcher>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication twoKey(argc, argv);
    MainWindow mainWindow;

    USBEventHandler usb_notif;
    twoKey.eventDispatcher()->installNativeEventFilter(&usb_notif);

    mainWindow.show();
    return twoKey.exec();
}
