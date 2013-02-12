#include "router.h"

Router::Router(QObject *parent) :
    QObject(parent)
{
}

Router::Router(QString mask)
{
    this->mask = mask;
}

RequestHeader Router::match(QString requestString)
{
    QRegExp byMask("<(\\w+)>");
    RequestHeader header;

    int pos = 0;

    while ((pos = byMask.indexIn(mask, pos)) != -1)
    {
        //list << byMask.cap(1);
        pos += byMask.matchedLength();
    }
}

