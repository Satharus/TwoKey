#ifndef USB_COMMUNICATOR_H
#define USB_COMMUNICATOR_H

#include "usb_event_handler.h"

#include <QSerialPort>


class USB_communicator
{
public:
    USB_communicator();
    ~USB_communicator();

    void write();
    QString read();
    void checkForToken();
    bool getTokenStatus();

    //Pointer to the USB handler, to get updates from it
    USBEventHandler *usb_notif;
    QString getMessage() const;
    QString getResponse() const;
private:

    QSerialPort *token;
    char message[17];
    QString response;
};

#endif // USB_COMMUNICATOR_H
