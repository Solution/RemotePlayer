#include "httpcontroler.h"

HttpControler::HttpControler(QObject *parent) :
    QTcpServer(parent)
{
}


void HttpControler::runServer(int port)
{
    if(!this->listen(QHostAddress::Any, port))
    {
        qDebug() << "Couldn't start server";
        qDebug() << this->errorString();
    }
    else{
        qDebug() << "Listening";
    }
}

void HttpControler::incomingConnection(int handle)
{
    ControlThread *thread = new ControlThread(handle, this);
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->run();
}
