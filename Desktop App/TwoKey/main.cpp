#include "usb_event_handler.h"
#include "mainwindow.h"

#include <QAbstractEventDispatcher>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication twoKey(argc, argv);

    USBEventHandler *usb_notif = new USBEventHandler();
    twoKey.eventDispatcher()->installNativeEventFilter(usb_notif);

    MainWindow mainWindow(nullptr, usb_notif);

    mainWindow.show();
    return twoKey.exec();
}
