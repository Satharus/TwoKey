#include "usb_communicator.h"

USBCommunicator::USBCommunicator()
{
    this->usb_notif = new USBEventHandler();
    this->token = new QSerialPort();

    for (int i = 0; i < 16; i++)
       message[i] = 'a' + static_cast<char>(i);
    message[16] = static_cast<char>(NULL);
    response = "";
}

USBCommunicator::~USBCommunicator()
{
    delete token;
    delete usb_notif;
}

void USBCommunicator::writeToToken(const char* message, int waitTime, bool flushAfterWrite)
{
    if (!token->isOpen()) return;
    strncpy(this->message, message, 16);
    token->write(message);
    token->waitForBytesWritten(waitTime);
    if (flushAfterWrite)
        token->flush();
}

QString USBCommunicator::readFromToken(int waitTime, bool flushAfterRead)
{
    if (!token->isOpen()) return "";
    token->waitForReadyRead(waitTime);
    response = token->read(32);
    if (flushAfterRead)
        token->flush();
    return response;
}

void USBCommunicator::clearTokenBuffer()
{
    token->flush();
    token->clear(QSerialPort::Direction::AllDirections);
    token->waitForReadyRead(1000);
}

void USBCommunicator::closeToken()
{
    this->clearTokenBuffer();
    this->token->close();
}


void USBCommunicator::checkForToken()
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

        token->flush();
        qDebug() << "Port: " << token->portName() << "Last Response: " << response;
    }
    else
    {
        qDebug() << "Token not present";
        token->close();
    }

}

bool USBCommunicator::getTokenStatus()
{
    return usb_notif->getTokenStatus();
}

QString USBCommunicator::getLastMessage() const
{
    QString msg(message);
    return msg;
}

QString USBCommunicator::getLastResponse() const
{
    return response;
}
