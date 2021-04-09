#include "usb_communicator.h"
#include "twokey.h"

#include <QAbstractEventDispatcher>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication twoKey(argc, argv);

    USBCommunicator *usb_comm = new USBCommunicator();
    twoKey.eventDispatcher()->installNativeEventFilter(usb_comm->usb_notif);

    TwoKey twoKeyWindow(nullptr, usb_comm);

    twoKeyWindow.show();
    return twoKey.exec();
}
