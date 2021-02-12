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

void USB_communicator::writeToToken(const char* message, int waitTime, bool flushAfterWrite)
{
    if (!token->isOpen()) return;
    strncpy(this->message, message, 16);
    token->write(message);
    token->waitForBytesWritten(waitTime);
    if (flushAfterWrite)
        token->flush();
}

QString USB_communicator::readFromToken(int waitTime, bool flushAfterRead)
{
    if (!token->isOpen()) return "";
    token->waitForReadyRead(waitTime);
    response = token->read(16);
    if (flushAfterRead)
        token->flush();
    return response;
}

void USB_communicator::clearTokenBuffer()
{
    token->flush();
    token->clear(QSerialPort::Direction::AllDirections);
    token->waitForReadyRead(1000);
}

void USB_communicator::closeToken()
{
    this->clearTokenBuffer();
    this->token->close();
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
        //Temporary hello message
        token->write(message);
        token->waitForBytesWritten(3000);
        qDebug() << token->bytesAvailable();

        if (token->bytesAvailable() > 0)
        {
            token->waitForReadyRead(3000);
            response = token->read(16);
            token->flush();
        }
        */
        token->flush();
        qDebug() << "Port: " << token->portName() << "Last Response: " << response;
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

QString USB_communicator::getLastMessage() const
{
    QString msg(message);
    return msg;
}

QString USB_communicator::getLastResponse() const
{
    return response;
}
