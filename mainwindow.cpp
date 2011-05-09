#include "mainwindow.h"
#include <QPixmap>
#include <QTimer>
#include <QDebug>
#include <QAction>
#include <QMenuBar>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    paused_ = false;

    pScene_ = new QGraphicsScene ();
    pView_  = new QGraphicsView ();

    QPixmap waves (":/pix/meri.png");
    pScene_->setBackgroundBrush(QBrush(waves));

    pTursas_ = new OrientationControlledGraphicsPixmapObject(QPixmap(":/pix/tursas.png"));
    pScene_->addItem(pTursas_);

    pView_->setScene(pScene_);
    setCentralWidget(pView_);


    //the boundaries of the scene are set to match the size of the view window, which is not
    //available in the constructor --> timer needed
    QTimer::singleShot(60,this,SLOT(initializeBoundaries()));


    QAction * pPauseAction = new QAction(tr("Pause"),this);
    pPauseAction->setCheckable(true);
    addAction(pPauseAction);
    connect(pPauseAction,SIGNAL(triggered(bool)),this,SLOT(pause(bool)));
    menuBar()->addAction(pPauseAction);

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
    pTursas_->setBoundaries(rectangle);
    pTursas_->startMoving();

    qDebug() << "Initialized boundaries" << rectangle.left() << rectangle.right() << pView_->width();
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
