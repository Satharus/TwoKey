#ifndef BROWSEREXTENSIONCOMMUNICATOR_H
#define BROWSEREXTENSIONCOMMUNICATOR_H

#include "backend_client.h"
#include "sandbird.h"

#include <cstring>
#include <cstdlib>
#include <QObject>
#include <QDebug>

class BrowserExtensionCommunicatorSignalWrapper : public QObject
{
    Q_OBJECT
public:
    BrowserExtensionCommunicatorSignalWrapper(BackendClient *backendClient);
    void emitSuccessfulLogin();

    BackendClient *getBackendClient() const;

signals:
    void successfulLogin();

private:
    BackendClient *backendClient;
};


class BrowserExtensionCommunicator : public QObject
{
    Q_OBJECT
public:
    BrowserExtensionCommunicator(BrowserExtensionCommunicatorSignalWrapper *signalWrapper);
    ~BrowserExtensionCommunicator();
    static int event_handler(sb_Event *e);

public slots:
    void startServer();
    void stopServer();

private:
    sb_Options opt;
    sb_Server *server;
    BrowserExtensionCommunicatorSignalWrapper *signalWrapper;
    bool running;
};

#endif // BROWSEREXTENSIONCOMMUNICATOR_H
