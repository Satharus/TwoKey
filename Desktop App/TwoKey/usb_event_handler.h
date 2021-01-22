#ifndef USB_EVENT_NOTIFICATION_H
#define USB_EVENT_NOTIFICATION_H


#include <QAbstractNativeEventFilter>
#include <QSerialPortInfo>
#include <QSerialPort>
#include <QObject>
#include <QWindow>
#include <QDebug>

#ifdef Q_OS_WIN32
#include <windows.h>
#include <dbt.h>
#endif

//Currently the token IDs are set to that of a Chinese Arduino Nano I had around.
#define TOKEN_VENDOR_ID     6790
#define TOKEN_PRODUCT_ID    29987


class USBEventHandler : public QObject, public QAbstractNativeEventFilter
{
    Q_OBJECT
public:
    explicit USBEventHandler(QObject *parent = nullptr);
    //Implementing the original function from QAbstractNativeEventFilter
    virtual bool nativeEventFilter(const QByteArray &eventType, void *message, long *result);

//Create signals for the states of USB devices
signals:
    void SerialDeviceInserted();
    void SerialDeviceRemoved();

private slots:
    void checkForToken();
};

#endif // USB_EVENT_NOTIFICATION_H
