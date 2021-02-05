#include "usb_communicator.h"

USB_communicator::USB_communicator()
{
    this->usb_notif = new USBEventHandler();
    this->token = new QSerialPort();

    for (int i = 0; i < 16; i++)
       message[i] = 'a' + static_cast<char>(i);
    message[16] = static_cast<char>(NULL);
    response = "";
}

USB_communicator::~USB_communicator()
{
    delete token;
    delete usb_notif;
}


void USB_communicator::checkForToken()
{
    if (usb_notif->getTokenStatus())
    {
        qDebug() << "Token present";
        if (!token->isOpen())
        {
           //Set connection properties for the token
           token->setPortName(usb_notif->getTokenPortName());
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


        token->write(message);
        token->waitForBytesWritten(3000);
        qDebug() << token->bytesAvailable();

        if (token->bytesAvailable() > 0)
        {
            token->waitForReadyRead(3000);
            response = token->read(16);
            token->flush();
        }

        qDebug() << "Port: " << token->portName() << "Value: " << response;
//         for (int i = 0; i < 16; i++)
//         {
//            qDebug() << static_cast<int>(message[i]) << '\t' << static_cast<int>(response[i]);
//         }
    }
    else
    {
        qDebug() << "Token not present";
        token->close();
    }

}

bool USB_communicator::getTokenStatus()
{
    return usb_notif->getTokenStatus();
}

QString USB_communicator::getMessage() const
{
    QString temp(message);
    return temp;
}

QString USB_communicator::getResponse() const
{
    return response;
}
