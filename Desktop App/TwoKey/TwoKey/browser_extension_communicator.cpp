#include "browser_extension_communicator.h"

BrowserExtensionCommunicatorSignalWrapper::BrowserExtensionCommunicatorSignalWrapper(BackendClient *backendClient)
{
    this->backendClient = backendClient;
}

void BrowserExtensionCommunicatorSignalWrapper::emitSignal(QString pwd)
{
    (void) pwd;
    emit testSignal(QString(this->backendClient->getJwt()));
}

BackendClient *BrowserExtensionCommunicatorSignalWrapper::getBackendClient() const
{
    return backendClient;
}

BrowserExtensionCommunicator::BrowserExtensionCommunicator(BrowserExtensionCommunicatorSignalWrapper *signalWrapper) :
    QObject()
{
    memset(&opt, 0, sizeof(opt));
    opt.port = "8000";
    opt.handler = event_handler;

    this->signalWrapper = signalWrapper;
    opt.udata = this->signalWrapper;

    server = sb_new_server(&opt);

    if (!server)
    {
      fprintf(stderr, "failed to initialize server\n");
      exit(EXIT_FAILURE);
    }
}

BrowserExtensionCommunicator::~BrowserExtensionCommunicator()
{
    if (running) this->stopServer();
    delete signalWrapper;
    free(server);
}

void BrowserExtensionCommunicator::startServer()
{
    running = true;
    qDebug() << "Server running at http://localhost:" + QString(opt.port);

    while(true)
    {
      sb_poll_server(server, 1000);
    }
}

void BrowserExtensionCommunicator::stopServer()
{
    running = false;
    sb_close_server(server);
}

/* Test url
 * http://localhost:8000/?masterpasswd=test234&email=newuser@gmail.com
 */
int BrowserExtensionCommunicator::event_handler(sb_Event *e)
{
    BrowserExtensionCommunicatorSignalWrapper* signalWrapper = (BrowserExtensionCommunicatorSignalWrapper*)e->udata;
    if (e->type == SB_EV_REQUEST)
    {
        char *masterpasswd = reinterpret_cast<char*>(malloc(1000));
        sb_get_var(e->stream, "masterpasswd", masterpasswd, 1000);

        char *email = reinterpret_cast<char*>(malloc(1000));
        sb_get_var(e->stream, "email", email, 1000);

        char *url = reinterpret_cast<char*>(malloc(1000));
        sb_get_var(e->stream, "url", url, 1000);

        qDebug() << e->method << "-" << e->path;
        qDebug() << "mstrpwd:\t" << masterpasswd;
        qDebug() << "url:\t" << url;
        qDebug() << "email:\t" << email;


        if (strlen(email) && strlen(masterpasswd) && signalWrapper->getBackendClient()->login(QString(email), QString(masterpasswd)))
        {
            sb_send_status(e->stream, 200, "OK");
            sb_send_header(e->stream, "Content-Type", "text/plain");
            signalWrapper->emitSignal(QString(masterpasswd));
            sb_writef(e->stream, signalWrapper->getBackendClient()->getJwt().toStdString().c_str());
        }
        else if (!strlen(url))
        {
            sb_send_status(e->stream, 401, "Unauthorised");
            sb_send_header(e->stream, "Content-Type", "text/plain");
            sb_writef(e->stream, "eskot ya 8aby, el password 8alat 😡");
        }

        if (strlen(url))
        {
            sb_send_status(e->stream, 200, "OK");
            sb_send_header(e->stream, "Content-Type", "text/plain");
            sb_writef(e->stream, "user=lol&pass=strongPass");
        }


        free (masterpasswd);
        free (url);
        free (email);
    }
    return SB_RES_OK;
}
