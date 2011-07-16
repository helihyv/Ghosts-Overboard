/**************************************************************************
        Ghosts Overboard - a game for Maemo 5

        Copyright (C) 2011  Heli Hyvättinen

        This file is part of Ghosts Overboard

        Ghosts Overboard is free software: you can redistribute it and/or modify
        it under the terms of the GNU General Public License as published by
        the Free Software Foundation, either version 2 of the License, or
        (at your option) any later version.

        This program is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
        GNU General Public License for more details.

        You should have received a copy of the GNU General Public License
        along with this program.  If not, see <http://www.gnu.org/licenses/>.

**************************************************************************/

#include "mainwindow.h"
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
    setWindowIcon(QIcon(":/pix/laiva_3aave.png"));
    setWindowTitle("Ghosts Overboard");

    pScene_ = new SeaScene ();
    connect(pScene_,SIGNAL(allGhostsPicked()),this,SLOT(nextLevel()));

    pView_  = new SeaView ();


    pView_->setScene(pScene_);
    setCentralWidget(pView_);

    pPauseAction_ = new QAction(tr("Pause"),this);
    pPauseAction_->setCheckable(true);
    addAction(pPauseAction_);
    connect(pPauseAction_,SIGNAL(toggled(bool)),pScene_,SLOT(pause(bool)));
    menuBar()->addAction(pPauseAction_);
    connect(pView_,SIGNAL(pauseChanged()),pPauseAction_,SLOT(toggle()));

    QAction * pRestartLevelAction = new QAction(tr("Restart level"),this);
    addAction(pRestartLevelAction);
    connect(pRestartLevelAction,SIGNAL(triggered()),this,SLOT(restartLevel()));
    menuBar()->addAction(pRestartLevelAction);

    QAction * pRestartGameAction = new QAction(tr("Restart game"),this);
    addAction(pRestartGameAction);
    connect(pRestartGameAction,SIGNAL(triggered()),this,SLOT(restartGame()));
    menuBar()->addAction(pRestartGameAction);

    pVibrateAction_ = new QAction(tr("Vibration effects"),this);
    pVibrateAction_->setCheckable(true);
    addAction(pVibrateAction_);
    connect(pVibrateAction_,SIGNAL(toggled(bool)),pScene_,SLOT(vibrationActivate(bool)));
    menuBar()->addAction(pVibrateAction_);


    QAction * pAboutAction = new QAction(tr("About"),this);
    addAction(pAboutAction);
    connect(pAboutAction,SIGNAL(triggered()),this,SLOT(about()));
    menuBar()->addAction(pAboutAction);



    Level level1(5,10);
    levelList_.append(level1);
    Level level2(5,10,2,50);
    levelList_.append(level2);
    Level level3(5,15,2,50);
    levelList_.append(level3);
    Level level4(5,15,4,50);
    levelList_.append(level4);
    Level level5(5,15,5,100);
    levelList_.append(level5);

    currentLevel_ = 0;


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

    //this occasionally gives a tiny scene, so using a fixed size fit for N900/Maemo5 until a fix is found

//    QPoint topleft (0,0);
//    QSize windowsize = pView_->size();
//    QRectF rectangle (topleft,windowsize);

    QRectF rectangle(0,0,800,424);

    pScene_->setSceneRect(rectangle);
    pView_->setSceneRect(rectangle);

    // qDebug() << "Initialized boundaries" << rectangle.right() << rectangle.bottom() << pView_->width() << pView_->height();

    restartLevel();
}


void MainWindow::restartLevel()
{
    pScene_->setupMap(levelList_.value(currentLevel_));  //value() returns default constructor Level if index is invalid, so no risk of crash
    pScene_->vibrationActivate(pVibrateAction_->isChecked());  //Vibration effects are lost without this
   // qDebug() << pVibrateAction_->isChecked();
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About %1").arg(QApplication::applicationName()),
                       tr("Version %1"
                          "<p>Copyright 2011 Heli Hyv&auml;ttinen"
                          "<p>License: General Public License v2"
                          "<p>Bug Reports: https://bugs.maemo.org/ "
                          "enter_bug.cgi?product=Ghosts%20Overboard"
                          ).arg(QApplication::applicationVersion()));



}

void MainWindow::nextLevel()
{

    currentLevel_++;

    if (levelList_.empty())
        pScene_->setupMap(Level());


    if ( currentLevel_ < levelList_.size() )
    {
       restartLevel();
    }

    else //Victory!
    {

       QDialog* pVictoryDialog = new QDialog(this);
       pVictoryDialog->setWindowTitle(tr("You won!"));


       QPushButton* pPlayAgainButton = new QPushButton(tr("Play again"));
//       QPushButton* pQuitButton = new QPushButton(tr("Quit game"));

       QPixmap victoryIcon (":/pix/aavesaari.png");
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

        //Never mind if the user cancels the dialog: restart the game anyway

       restartGame();
    }
}

bool MainWindow::event(QEvent *event)
{

    switch (event->type())
    {
        //pause if app goes to background
        case QEvent::WindowDeactivate:

            if (pScene_)
                pScene_->pause(true);
            break;

        //un-pause if app gomes back to foreground unless it was paused before going to background
        case QEvent::WindowActivate:


            if (pPauseAction_ && !pPauseAction_->isChecked())
            {
                if (pScene_)
                    pScene_->pause(false);
            }
            break;

        //Just to keep the compiler from complaining...
        default:
            break;

     }



    //pass the event to the ancestor for handling
    return QMainWindow::event(event);

 }

void MainWindow::restartGame()
{
    currentLevel_ = 0;
    restartLevel();
}
