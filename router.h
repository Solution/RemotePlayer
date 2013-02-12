#ifndef ROUTER_H
#define ROUTER_H

#include "controllerrequest.h"
#include <QObject>
#include <QRegExp>

class Router : public QObject
{
    Q_OBJECT
public:
    explicit Router(QObject *parent = 0);
    Router(QString mask);

    RequestHeader match(QString requestString);
signals:
    
public slots:
    
private:
    QString mask;
};

#endif // ROUTER_H
