#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowState(Qt::WindowFullScreen);

    player = new Phonon::VideoPlayer(Phonon::VideoCategory, ui->centralWidget);

    QSize screenSize = QApplication::desktop()->screenGeometry().size();

    player->setMinimumSize(screenSize.width(), screenSize.height());
    /*int centerX = (int) (player->width() - (screenSize.width()/2));
    int centerY = (int) (player->height() - (screenSize.height()/2));*/

    int posX = QApplication::desktop()->screenGeometry(QApplication::desktop()->primaryScreen()).x();
    move(posX, 0);
    //player->move(centerX, centerY-150);

    label = new QLabel(this);
    label->setMinimumSize(1280, 50);
    //label->move(centerX, ((centerY-130) + player->height()));

    QFont font;
    font.setPixelSize(20);
    font.setBold(true);

    label->setFont(font);

    setObjectName("Remote player by Solution");

}

QLabel *MainWindow::getLabel()
{
    return label;
}

void MainWindow::setRouter(Router *router)
{
   if(!this->router) this->router = router;
}

Router *MainWindow::getRouter()
{
    return router;
}

Phonon::VideoPlayer *MainWindow::getPlayer()
{
    return player;
}

void MainWindow::run()
{
    controller = new HttpControler(this);
    controller->runServer(3333);
    emit show();
}

MainWindow::~MainWindow()
{
    delete ui;
}
