#ifndef BROWSEREXTENSIONCOMMUNICATOR_H
#define BROWSEREXTENSIONCOMMUNICATOR_H

#include "sandbird.h"

#include <cstring>
#include <cstdlib>
#include <QObject>
#include <QDebug>

class BrowserExtensionCommunicatorEmitter : public QObject
{
    Q_OBJECT
public:
    BrowserExtensionCommunicatorEmitter();
    void emitSignal(QString pwd);
signals:
    void testSignal(QString pwd);
};


class BrowserExtensionCommunicator : public QObject
{
    Q_OBJECT
public:
    BrowserExtensionCommunicator(BrowserExtensionCommunicatorEmitter *emitter);
    ~BrowserExtensionCommunicator();
    static int event_handler(sb_Event *e);

public slots:
    void startServer();
    void stopServer();

private:
    sb_Options opt;
    sb_Server *server;
    BrowserExtensionCommunicatorEmitter *emitter;
    bool running;
};

#endif // BROWSEREXTENSIONCOMMUNICATOR_H
