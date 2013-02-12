#include <QtGui/QApplication>
#include "router.h"
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("Video player");
    MainWindow w;
    w.setRouter(new Router("<presenter>/<action>/<id>"));
    w.run();
    
    return a.exec();
}
