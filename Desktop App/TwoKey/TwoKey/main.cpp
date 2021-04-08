#include "usb_communicator.h"
#include "twokey.h"

#include <QAbstractEventDispatcher>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication twoKey(argc, argv);

    USB_communicator *usb_comm = new USB_communicator();
    twoKey.eventDispatcher()->installNativeEventFilter(usb_comm->usb_notif);

    TwoKey twoKeyWindow(nullptr, usb_comm);

    twoKeyWindow.show();
    return twoKey.exec();
}
