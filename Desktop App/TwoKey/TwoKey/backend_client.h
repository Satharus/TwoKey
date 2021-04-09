#ifndef BACKEND_CLIENT_H
#define BACKEND_CLIENT_H

#include "usb_communicator.h"

#include <QtNetwork>


class BackendClient
{
public:
    BackendClient(USBCommunicator *usbComm);
    ~BackendClient();
    bool login(QString email, QString password);
    bool _register(QString firstName, QString lastName, QString email, QString username, QString password, QString serial);
    bool _2fa();
    bool logout();
    QString getJwt() const;

private:
    QString jwt;
    QString tokenChallengeResponse;
    USBCommunicator *usbComm;
};

#endif // BACKEND_CLIENT_H
