#ifndef CONTROLLERREQUEST_H
#define CONTROLLERREQUEST_H

#include <QObject>
#include <QString>
#include <QMap>

struct RequestHeader {
    QString controller;
    QString action;
    QMap<QString, QString> params;
};

class ControllerRequest : public QObject
{
    Q_OBJECT
public:
    explicit ControllerRequest(QObject *parent = 0);
    ControllerRequest(RequestHeader request);
    
    void call();
signals:
    
public slots:
    
};

#endif // CONTROLLERREQUEST_H
