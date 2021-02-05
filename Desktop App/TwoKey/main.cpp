#include "usb_communicator.h"
#include "mainwindow.h"

#include <QAbstractEventDispatcher>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication twoKey(argc, argv);

    USB_communicator *usb_comm = new USB_communicator();
    twoKey.eventDispatcher()->installNativeEventFilter(usb_comm->usb_notif);

    MainWindow mainWindow(nullptr, usb_comm);

    mainWindow.show();
    return twoKey.exec();
}
