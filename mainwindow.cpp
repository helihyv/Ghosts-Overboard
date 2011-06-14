#include "mainwindow.h"
#include "timercontrolledtursas.h"
#include <QPixmap>
#include <QTimer>
#include <QDebug>
#include <QAction>
#include <QMenuBar>
#include <QMessageBox>
#include <QApplication>
#include <QLabel>
#include <QPushButton>
 #include <QVBoxLayout>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    paused_ = false;

    setWindowIcon(QIcon(":/pix/laiva_10aave.png"));

    pScene_ = new SeaScene ();
    connect(pScene_,SIGNAL(allGhostsPicked()),this,SLOT(nextLevel()));

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

    restartLevel();
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

void MainWindow::nextLevel()
{

    //for now, just the handling of last level is implemented, and there is just one level

    qDebug() << "starting game over";
       QPixmap victoryIcon (":/pix/aavesaari.png");
//    QMessageBox victoryBox(QMessageBox::Information, tr("You won1"), tr("Congratulations! You have saved all the ghosts."));

//    victoryBox.setIconPixmap(victoryIcon);
//    victoryBox.addButton("Start a new game",QMessageBox::YesRole);
//    victoryBox.addButton("Quit",QMessageBox::NoRole);

//    victoryBox.exec();


       QDialog* pVictoryDialog = new QDialog(this);
       pVictoryDialog->setWindowTitle(tr("You won!"));


       QPushButton* pPlayAgainButton = new QPushButton(tr("Play again"));
       QPushButton* pQuitButton = new QPushButton(tr("Quit game"));
       QLabel* pVictoryLabel = new QLabel();
       pVictoryLabel->setPixmap(victoryIcon);
       QLabel* pTextLabel = new QLabel(tr("Congratulations! <p>You have saved all the ghosts."));


       QVBoxLayout* pMainLayout = new QVBoxLayout;

       QHBoxLayout* pTopLayout = new QHBoxLayout;
       pMainLayout->addLayout(pTopLayout);

       pTopLayout->addWidget(pVictoryLabel);
       pTopLayout->addWidget(pTextLabel);



       QHBoxLayout* pButtonLayout = new QHBoxLayout();
       pMainLayout->addLayout(pButtonLayout);


 //      pButtonLayout->addWidget(pQuitButton);
       pButtonLayout->addWidget(pPlayAgainButton);



       pVictoryDialog->setLayout(pMainLayout);

       connect(pPlayAgainButton, SIGNAL(clicked()),pVictoryDialog,SLOT(accept()));

       pVictoryDialog->exec();

       restartLevel();



}
