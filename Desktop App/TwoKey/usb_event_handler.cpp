#include "usb_event_handler.h"
#include "mainwindow.h"

USBEventHandler::USBEventHandler(QObject *parent) : QObject(parent)
{
    token = new QSerialPort();
#ifdef Q_OS_LINUX
    timer = new QTimer(this);

    udev = udev_new();
    if (!udev)
        qDebug() << "Failed to allocate new udev";

    //Initialise the timeout for Udevs pooling
    udevTimeout.tv_sec = 1;
    udevTimeout.tv_usec = 0;
    //Initialise the udev monitor in order to be able to pool the devices
    mon = udev_monitor_new_from_netlink(udev, "udev");
    //Add a filter to get only USB devices
    udev_monitor_filter_add_match_subsystem_devtype(mon, "usb", NULL);
    //Enable receiving for the monitor
    udev_monitor_enable_receiving(mon);

    //Connect the timer's timeout with the tick function, to check every X ms
    connect(timer, SIGNAL(timeout()), this, SLOT(tick()));
    timer->start(250/*X*/);
#endif
    //Check for the token every time a device is inserted or removed
    connect(this, SIGNAL(SerialDeviceInserted()), SLOT(checkForToken()));
    connect(this, SIGNAL(SerialDeviceRemoved()), SLOT(checkForToken()));
}


USBEventHandler::~USBEventHandler()
{
#ifdef Q_OS_LINUX
    udev_unref(udev);
    udev_monitor_unref(mon);
    timer->stop();
    delete timer;
#endif
    delete token;
}

bool USBEventHandler::nativeEventFilter(const QByteArray &eventType, void *_message, long *result)
{
    //Unused stuff, required for the function signature
    (void) eventType;
    (void) result;
#ifdef Q_OS_LINUX
    (void) _message;
#endif
#ifdef Q_OS_WIN
    //Get the message and interpret it
    MSG *message = static_cast<MSG*>(_message);

    //Get the device structure
    PDEV_BROADCAST_HDR device = reinterpret_cast<PDEV_BROADCAST_HDR>(message->lParam);

    //If device is inserted
    if (message->message == WM_DEVICECHANGE && message->wParam == DBT_DEVICEARRIVAL)
    {
        qDebug() << "Device Inserted, type: " << device->dbch_devicetype;
        if (device->dbch_devicetype == 0x3)
            emit SerialDeviceInserted();
    }
    else if (message->message == WM_DEVICECHANGE && message->wParam == DBT_DEVICEREMOVECOMPLETE)
    {
        qDebug() << "Device Removed, type: " << device->dbch_devicetype;
        if (device->dbch_devicetype == 0x3)
          emit SerialDeviceRemoved();
    }
#endif
    return false;
}

void USBEventHandler::checkForToken()
{
     qDebug() << "Checking for token: ";
     bool tokenIsAvailable = false;
     QString tokenPortName;

     //Enumerate all of the connected serial devices and check if it is the token
     foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts())
     {
         //Check if it matches the vendor and product ID of the token
         if (serialPortInfo.hasVendorIdentifier() &&
                 serialPortInfo.hasProductIdentifier() &&
                 serialPortInfo.vendorIdentifier() ==  TOKEN_VENDOR_ID &&
                 serialPortInfo.productIdentifier() == TOKEN_PRODUCT_ID)
         {
             tokenPortName = serialPortInfo.portName();
             tokenIsAvailable = true;
         }
     }

     //Alert the rest of the classes that the token status has changed, and update its status
     tokenConnected = tokenIsAvailable;
     emit this->tokenStatusChanged();

     if (tokenIsAvailable)
     {
         qDebug() << "Token present";
         if (!token->isOpen())
         {
            //Set connection properties for the token
            token->setPortName(tokenPortName);
            token->setBaudRate(QSerialPort::Baud9600);
            token->setDataBits(QSerialPort::Data8);
            token->setParity(QSerialPort::NoParity);
            token->setStopBits(QSerialPort::OneStop);
            token->setFlowControl(QSerialPort::NoFlowControl);
            if (!token->open(QIODevice::ReadWrite))
                qDebug() << "Couldn't communicate with token, it is likely in use by another program.";
         }
         /*
             TODO: Fix the communication between this part and the arduino,
             it doesn't seem to work properly (timing wise)
             Later on, the actual communication with the token will be
         */
         //
         token->write("1");
         token->waitForBytesWritten();
         token->waitForReadyRead(2000);

         QString value = token->readAll();
         qDebug() << "Port: " << token->portName() << "Value: " << value;

         token->clear(QSerialPort::Direction::AllDirections);
     }
     else
         qDebug() << "Token not present";
}

#ifdef Q_OS_LINUX
void USBEventHandler::tick()
{
    //Get file descriptor for events from udev
    deviceFD = udev_monitor_get_fd(mon);
    //Create a file descriptor set
    fd_set fds;
    //Initialise to zero
    FD_ZERO(&fds);
    //Populate the set
    FD_SET(deviceFD, &fds);

    //Retrieve the events
    int ret = select(deviceFD+1, &fds, NULL, NULL, &udevTimeout);
    if (ret <= 0)
        return;

    //Check if the descriptor is in the set
    if (FD_ISSET(deviceFD, &fds))
    {
        //Get the device from the monitor
        struct udev_device* dev = udev_monitor_receive_device(mon);
        if (dev)
        {
            //Get the event from the device
            if (udev_device_get_devnode(dev))
            {
                const char* action = udev_device_get_action(dev);
                //Emit the signals accordingly
                if (strcmp(action, "add") == 0)
                    emit SerialDeviceInserted();
                else if (strcmp(action, "remove") == 0)
                    emit SerialDeviceRemoved();
                /*
                const char* vendor = udev_device_get_sysattr_value(dev, "idVendor");
                const char* product = udev_device_get_sysattr_value(dev, "idProduct");
                qDebug() << udev_device_get_subsystem(dev)
                         << udev_device_get_devtype(dev)
                         << action
                         << vendor
                         << product
                         << udev_device_get_devnode(dev);
                */
            }
        }
    }
}
#endif
