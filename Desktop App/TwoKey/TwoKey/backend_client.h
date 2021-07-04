#ifndef BACKEND_CLIENT_H
#define BACKEND_CLIENT_H

#include "usb_communicator.h"

#include <QCryptographicHash>
#include <QtNetwork>

class BackendClient : public QObject
{
    Q_OBJECT
public:
    BackendClient(USBCommunicator *usbComm);
    int login(QString email, QString password);
    bool _register(QString firstName, QString lastName, QString email, QString username, QString password, QString serial);
    int _2fa();
    bool logout();
    QString getJwt() const;

    enum loginStatus
    {
        SUCCESS       =     0,
        INVALID       =     1,
        DOESNT_EXIST  =     2
    };
    Q_ENUM(loginStatus)

private:
    QString jwt;
    QString tokenChallengeResponse;
    USBCommunicator *usbComm;
};

#endif // BACKEND_CLIENT_H
