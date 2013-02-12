#include "controlthread.h"

ControlThread::ControlThread(int id, QObject *parent) :
    QThread(parent),
    player(((Phonon::VideoPlayer*) ((MainWindow*) this->parent()->parent())->getPlayer())),
    label(((QLabel*) ((MainWindow*) this->parent()->parent())->getLabel())),
    state(STOP),
    seekTime(0),
    volume(player->volume())
{
    socketDescriptor = id;

    // Televisions default
    tvs.insert("Nova", "http://tv.dupeto.cz/nova");
    tvs.insert("Cool", "http://tv.dupeto.cz/cool");
    tvs.insert("Prima", "http://tv.dupeto.cz/prima");
    tvs.insert("Love", "http://tv.dupeto.cz/love");
    tvs.insert("CT1", "http://tv.dupeto.cz/ct1");
    tvs.insert("CT2", "http://tv.dupeto.cz/ct2");
    tvs.insert("CT24", "http://tv.dupeto.cz/ct24");
    tvs.insert("NovaCinema", "http://tv.dupeto.cz/nova-cinema");
    tvs.insert("Barrandov", "http://tv.dupeto.cz/barrandov");
}

QString ControlThread::getActualPath(QString header)
{
    QStringList items = header.split("\n");
    QStringList subItems = items[0].split(" ");

    QStringList params = subItems[1].split("?");

    if(params.size() > 1)
    {
        QStringList volVal = params[1].split("=");
        if(params[1] == "stop")
        {
            state = STOP;
            player->stop();
            label->setText("");
        }
        else if(params[1] == "resume")
        {
            state = RESUME;
            player->play();
        }
        else if(params[1] == "play")
        {
            state = PLAY;
            if(player->mediaObject()->state() == Phonon::BufferingState) player->load(Phonon::MediaSource(params[0]));
            player->play();
            player->setVolume(volume);
        }else if(params[1].contains("setVolume"))
        {
            state = VOLUME;
            qDebug() << "Changing volume" << volVal[1];
            volume = volVal[1].toFloat();
            player->setVolume(volVal[1].toFloat());
        }else if(params[1].contains("playTv"))
        {
            state = PLAY;
            player->load(tvs.value(volVal[1]));
            player->play();
        }else if(params[1] == "pause")
        {
            state = PAUSE;
            player->pause();
            label->setText("Paused");
        }else if(params[1] == "fullscreen")
        {
            if(player->windowState() == Qt::WindowFullScreen)
            {
                player->showNormal();
            }
            else
            {
                player->showFullScreen();
            }
        }


        return params[0];
    }else{
        if(subItems[1] != "/favicon.ico")
        {
            state = STOP;
            player->update();
            qDebug() << "returning only path";

            if(!(QDir(subItems[1]).exists())) player->load(Phonon::MediaSource(subItems[1]));
        }
        return subItems[1];
    }
}

void ControlThread::listDirs(QString path)
{
    QDir dir(path);
    QFileInfoList list = dir.entryInfoList();  

    QString data;

    data.append("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
                "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Strict//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd\">\n"
                "<html xmlns=\"http://www.w3.org/1999/xhtml\" xml:lang=\"cs\" lang=\"cs\">\n"
                "<head>\n"
                "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" />\n"
                "<body>\n");

    data.append("<h1>Files</h1>");
    data.append("<ul>");
    for(int i = 0; i < list.size(); i++)
    {
        QFileInfo fileInfo = list.at(i);
        data.append(QString("<li><strong><a href=\"%1/%2\">%3</a> </strong></li>\n").arg(path == "/" ? "" : path).arg(Qt::escape(fileInfo.fileName())).arg(fileInfo.fileName()));
    }
    data.append("</ul>");

    data.append("<h3>Televisions: </h3>");
    data.append("<ul style=\"list-style: none;\">");

    QMapIterator<QString, QString> i(tvs);

    while(i.hasNext())
    {
        i.next();
        data.append(QString("<li style=\"display: inline;\"><a href=\"?playTv=%1\">%1</a> | </li>").arg(i.key()));
    }
    data.append("</ul>");

    data.append("<a href=\"?play\">Play</a> | <a href=\"?resume\">Resume</a> | <a href=\"?pause\">Pause</a> | <a href=\"?stop\">Stop</a>");

    if(player) data.append(QString("<br /><span>Volume: %1 %</span>").arg(100*volume));
    data.append("<br />Volumes: <a href=\"?setVolume=0.25\">25%</a> | <a href=\"?setVolume=0.5\">50%</a> | <a href=\"?setVolume=0.75\">75%</a> | <a href=\"?setVolume=1\">100%</a> | "
                "<a href=\"?setVolume=1.25\">125%</a> | <a href=\"?setVolume=1.5\">150%</a> | <a href=\"?setVolume=1.75\">175%</a>");

    data.append("</body></html>");

    QString header("HTTP/1.1 200 OK\nServer: RemotePlayer\nContent-Type: text/html; charset=utf-8\n");
    header.append(QString("Content-Length: %1\n\n").arg(data.size()));

    socket->write(header.toAscii());
    socket->write(data.toLocal8Bit());
    socket->close();

}

void ControlThread::readyRead()
{
    // todo
    Router *router = ((Router*) ((MainWindow*) this->parent()->parent())->getRouter());

    QString path(getActualPath(QString(socket->readAll()).toLocal8Bit()));
    qDebug() << path;
    if(QDir(path).exists())
    {
        listDirs(path);
    }
    else
    {
        if(path != "/favicon.ico")
        {
            QString data;

            data.append("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
                        "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Strict//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd\">\n"
                        "<html xmlns=\"http://www.w3.org/1999/xhtml\" xml:lang=\"cs\" lang=\"cs\">\n"
                        "<head>\n"
                        "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" />\n"
                        "<body>\n");
            data.append("<h2>Video control</h2>");
            data.append("<a href=\"?play\">Play</a> | <a href=\"?resume\">Resume</a> | <a href=\"?pause\">Pause</a> | <a href=\"?stop\">Stop</a>");

            if(player) data.append(QString("<br /><span>Volume: %1 %</span>").arg(100*volume));
            data.append("<br />Volumes: <a href=\"?setVolume=0.25\">25%</a> | <a href=\"?setVolume=0.5\">50%</a> | <a href=\"?setVolume=0.75\">75%</a> | <a href=\"?setVolume=1\">100%</a> | "
                        "<a href=\"?setVolume=1.25\">125%</a> | <a href=\"?setVolume=1.5\">150%</a> | <a href=\"?setVolume=1.75\">175%</a>");

            int offset = path.lastIndexOf("/");
            QString upPath = path.mid(0, offset);

            label->setText(QString("Playing: %1").arg(path.mid(offset, -1)));

            data.append(QString("<br /> <a href=\"%1\">To the files</a>").arg(upPath));
            if(player->mediaObject()->state() == Phonon::PlayingState) data.append(QString("<br /> Length of the video/audio: %1(minutes)").arg(((player->totalTime()/1000)/60)));
            data.append("<br /><a href=\"?refresh/\">Refresh</a>");
            data.append(QString("<br /><a href=\"?fullscreen\">Fullscreen</a>"));

            QString header("HTTP/1.1 200 OK\nServer: RemotePlayer\nContent-Type: text/html; charset=utf-8\n");
            header.append(QString("Content-Length: %1\n\n").arg(data.size()));

            socket->write(header.toLocal8Bit());
            socket->write(data.toLocal8Bit());
            socket->close();
            qDebug() << state;
        }else{
            socket->close();
        }
    }

    qDebug() << "socket closed";
}

void ControlThread::disconnected()
{
    socketDescriptor = -1;
}

void ControlThread::run()
{
    //todo
    if(socketDescriptor != -1)
    {
        socket = new QTcpSocket();
        socket->setSocketDescriptor(socketDescriptor);

        connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
        connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    }else{
        qDebug() << "Wrong socket descriptor id";
    }
}
