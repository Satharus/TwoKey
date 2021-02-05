#ifndef USB_EVENT_NOTIFICATION_H
#define USB_EVENT_NOTIFICATION_H


#include <QAbstractNativeEventFilter>
#include <QSerialPortInfo>
#include <QObject>
#include <QDebug>

#ifdef Q_OS_LINUX
#include <QTimer>
#include <string.h>
#include <libudev.h>
#endif

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
    //Constructor and Desctructor
    explicit USBEventHandler(QObject *parent = nullptr);
    ~USBEventHandler();

    //Implementing the original function from QAbstractNativeEventFilter
    virtual bool nativeEventFilter(const QByteArray &eventType, void *message, long *result);

    bool getTokenStatus();
    QString getTokenPortName();


signals:
    //Signals for the states of USB devices
    void SerialDeviceInserted();
    void SerialDeviceRemoved();
    //A signal to other classes(such as MainWindow)that the token has changed
    void tokenStatusChanged();

public slots:
    //Function checks if the token itself exists, and communicates with it
    void checkDeviceID();
#ifdef Q_OS_LINUX
    //Tick function to check for the token on Linux every unit time
    void tick();
#endif

private:
//Linux specific objects
#ifdef Q_OS_LINUX
    QTimer *timer;
    struct udev_monitor* mon;
    struct timeval udevTimeout;
    struct udev* udev;
    int deviceFD;
#endif
    //Variable to store the status of the token
    bool tokenIsAvailable;
    QString tokenPortName;
};

#endif // USB_EVENT_NOTIFICATION_H
