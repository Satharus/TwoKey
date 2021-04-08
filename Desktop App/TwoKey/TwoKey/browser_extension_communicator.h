#ifndef BROWSEREXTENSIONCOMMUNICATOR_H
#define BROWSEREXTENSIONCOMMUNICATOR_H

#include "sandbird.h"



#include <cstring>
#include <cstdlib>
#include <QObject>
#include <QDebug>


class BrowserExtensionCommunicator : public QObject
{
    Q_OBJECT
public:
    BrowserExtensionCommunicator();
    ~BrowserExtensionCommunicator();
    static int event_handler(sb_Event *e);

public slots:
    void startServer();
    void stopServer();

signals:
    void testSignal(QString pwd);

private:
    sb_Options opt;
    sb_Server *server;
    bool running;
};

#endif // BROWSEREXTENSIONCOMMUNICATOR_H
