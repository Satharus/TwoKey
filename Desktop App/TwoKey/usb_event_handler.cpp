#include "usb_event_handler.h"
#include "mainwindow.h"

USBEventHandler::USBEventHandler(QObject *parent) : QObject(parent)
{
    connect(this, SIGNAL(SerialDeviceInserted()), SLOT(checkForToken()));
    connect(this, SIGNAL(SerialDeviceRemoved()), SLOT(checkForToken()));
}

bool USBEventHandler::nativeEventFilter(const QByteArray &eventType, void *_message, long *result)
{
    //Unused stuff, required for the function signature
    (void) eventType;
    (void) result;

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
     QSerialPort *token = new QSerialPort();
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

     if (tokenIsAvailable)
     {
         qDebug() << "Token present";
         if (!token->isOpen())
         {
            token->setPortName(tokenPortName);
            token->setBaudRate(QSerialPort::Baud9600);
            token->setDataBits(QSerialPort::Data8);
            token->setParity(QSerialPort::NoParity);
            token->setStopBits(QSerialPort::OneStop);
            token->setFlowControl(QSerialPort::NoFlowControl);
            if (!token->open(QIODevice::ReadWrite))
            {
                qDebug() << "Couldn't communicate with token";
            }
         }
         /*
             TODO: Fix the communication between this part and the arduino,
             it doesn't seem to work properly (timing wise)
         */
         token->write("1");
         token->waitForBytesWritten();
         token->waitForReadyRead(2000);

         QString value = token->readAll();
         qDebug() << "Port: " << token->portName() << "Value: " << value;
         token->clear(QSerialPort::Direction::AllDirections);
     }
     else
     {
         qDebug() << "Token not present";
     }
}
