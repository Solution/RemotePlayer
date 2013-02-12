#ifndef CONTROLTHREAD_H
#define CONTROLTHREAD_H

#include <QThread>
#include <QTcpSocket>
#include <QDebug>
#include <QLabel>
#include "mainwindow.h"
#include "router.h"
#include <QDir>
#include <QFileInfoList>
#include <QTextDocument>
#include "phonon"

enum STATES { PLAY = 1, STOP = 2, RESUME = 3, VOLUME = 4, PAUSE = 5};

class ControlThread : public QThread
{
    Q_OBJECT
public:
    explicit ControlThread(int id, QObject *parent = 0);
    void run();

signals:
    
public slots:
    void readyRead();
    void disconnected();

private:
    int socketDescriptor;
    QTcpSocket *socket;
    void listDirs(QString path);
    QString getActualPath(QString header);
    Phonon::VideoPlayer *player;
    QLabel *label;
    QMap<QString, QString> tvs;

    int state;
    qint64 seekTime;
    float volume;
};

#endif // CONTROLTHREAD_H
