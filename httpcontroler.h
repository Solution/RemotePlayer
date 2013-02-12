#ifndef HTTPCONTROLER_H
#define HTTPCONTROLER_H

#include <QTcpServer>
#include "controlthread.h"

class HttpControler : public QTcpServer
{
    Q_OBJECT
public:
    explicit HttpControler(QObject *parent = 0);

    void runServer(int port);

signals:
    
public slots:
    void incomingConnection(int handle);
};

#endif // HTTPCONTROLER_H
