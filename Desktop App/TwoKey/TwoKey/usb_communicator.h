#ifndef USB_COMMUNICATOR_H
#define USB_COMMUNICATOR_H

#include "usb_event_handler.h"

#include <QSerialPort>


class USBCommunicator
{
public:
    USBCommunicator();
    ~USBCommunicator();

    void writeToToken(const char* message, int waitTime = 3000, bool flushAfterWrite = true);
    QString readFromToken(int waitTime = 3000, bool flushAfterRead = false);
    void clearTokenBuffer();
    void closeToken();
    void checkForToken();
    bool getTokenStatus();

    //Pointer to the USB handler, to get updates from it
    USBEventHandler *usb_notif;
    QString getLastMessage() const;
    QString getLastResponse() const;
private:

    QSerialPort *token;
    char message[17];
    QString response;
};

#endif // USB_COMMUNICATOR_H
