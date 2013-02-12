#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "router.h"
#include "httpcontroler.h"
#include <QMainWindow>
#include <QLabel>
#include <phonon/phonon>
#include <QApplication>
#include <QDesktopWidget>
#include <QFont>

class HttpControler;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
    QLabel *getLabel();

    void setRouter(Router *router);
    Router *getRouter();
    Phonon::VideoPlayer *getPlayer();

    void run();
private:
    Ui::MainWindow *ui;
    Router* router;
    HttpControler *controller;
    QLabel *label;
    Phonon::VideoPlayer *player;
};

#endif // MAINWINDOW_H
