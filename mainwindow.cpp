#include "mainwindow.h"
#include "timercontrolledtursas.h"
#include <QPixmap>
#include <QTimer>
#include <QDebug>
#include <QAction>
#include <QMenuBar>
#include <QMessageBox>
#include <QApplication>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    paused_ = false;

    setWindowIcon(QIcon(":/pix/laiva_10aave.png"));

    pScene_ = new SeaScene ();
    pView_  = new QGraphicsView ();



    pView_->setScene(pScene_);
    setCentralWidget(pView_);

    QAction * pPauseAction = new QAction(tr("Pause"),this);
    pPauseAction->setCheckable(true);
    addAction(pPauseAction);
    connect(pPauseAction,SIGNAL(triggered(bool)),this,SLOT(pause(bool)));
    menuBar()->addAction(pPauseAction);

    QAction * pRestartLevelAction = new QAction(tr("Restart level"),this);
    addAction(pRestartLevelAction);
    connect(pRestartLevelAction,SIGNAL(triggered()),this,SLOT(restartLevel()));
    menuBar()->addAction(pRestartLevelAction);


    QAction * pAboutAction = new QAction(tr("About"),this);
    addAction(pAboutAction);
    connect(pAboutAction,SIGNAL(triggered()),this,SLOT(about()));
    menuBar()->addAction(pAboutAction);


    //the boundaries of the scene are set to match the size of the view window, which is not
    //available in the constructor --> timer needed
    QTimer::singleShot(100,this,SLOT(initializeBoundaries()));






}

MainWindow::~MainWindow()
{

}

void MainWindow::initializeBoundaries()
{
        //the boundaries of the scene are set to match the size of the view window, and
        //the view is set to show exactly the whole scene area

    QPoint topleft (0,0);
    QSize windowsize = pView_->size();
    QRectF rectangle (topleft,windowsize);


    pScene_->setSceneRect(rectangle);
    pView_->setSceneRect(rectangle);

    qDebug() << "Initialized boundaries" << rectangle.right() << rectangle.bottom() << pView_->width() << pView_->height();

    pScene_->setupMap(11,5,5);
}

void MainWindow::pause(bool paused)
{
//    qDebug() << "pause pressed " << paused;
    if (paused_ == paused)
            return;

    paused_ = paused;

    if (paused == false)
    {
 //       qDebug() << "starting to move again";
        pTursas_->startMoving();
    }

    else
    {
        qDebug("about to stop movement");
        pTursas_->stopMoving();
    }

}

void MainWindow::restartLevel()
{
    pScene_->setupMap(5,5,5);
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About %1").arg(QApplication::applicationName()),
                       tr("Version %1"
                          "<p>Copyright 2011 Heli Hyv&auml;ttinen"
                          "<p>License: General Public License v2"
                          ).arg(QApplication::applicationVersion()));



}
