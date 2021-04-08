#include "browser_extension_communicator.h"

BrowserExtensionCommunicator::BrowserExtensionCommunicator() :
    QObject()
{
    memset(&opt, 0, sizeof(opt));
    opt.port = "8000";
    opt.handler = event_handler;

    server = sb_new_server(&opt);

    if (!server) {
      fprintf(stderr, "failed to initialize server\n");
      exit(EXIT_FAILURE);
    }
}

BrowserExtensionCommunicator::~BrowserExtensionCommunicator()
{
    if (running) this->stopServer();
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

int BrowserExtensionCommunicator::event_handler(sb_Event *e) {
  if (e->type == SB_EV_REQUEST)
  {
    char *passwd = reinterpret_cast<char*>(malloc(20));
    sb_get_var(e->stream, "masterpwd", passwd, 20);

    char *url = reinterpret_cast<char*>(malloc(1000));
    sb_get_var(e->stream,"url",url,1000);

    char *username = reinterpret_cast<char*>(malloc(20));
    sb_get_var(e->stream,"email",username,20);

     char *Pwd = reinterpret_cast<char*>(malloc(20));
    sb_get_var(e->stream,"pwd",Pwd,20);

    qDebug() << e->method << "-" << e->path;
    qDebug() << "mstrpwd:\t" << passwd;
    qDebug() << "url:\t" << url;
    qDebug() << "email:\t" << username;
    qDebug() << "pwd:\t" << Pwd;
    if (!strcmp(passwd, "lololol"))
    {
        sb_send_status(e->stream, 200, "OK");
        sb_send_header(e->stream, "Content-Type", "text/plain");
        sb_writef(e->stream, "user:omar;pass=lol");
    }
    else
    {
        sb_send_status(e->stream, 200, "OK");
        sb_send_header(e->stream, "Content-Type", "text/plain");
        sb_writef(e->stream, "eskot ya 8aby");
    }

  }
  return SB_RES_OK;
}



