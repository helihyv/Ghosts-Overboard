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
    connect(pScene_,SIGNAL(allGhostsPicked()),pScene_,SLOT(nextLevel()));

    pView_  = new SeaView ();


    pView_->setScene(pScene_);
    setCentralWidget(pView_);

    connect(pView_,SIGNAL(screenTapped()),pScene_,SLOT(handleScreenTapped()));
    connect(pView_,SIGNAL(goingBackgroung()),pScene_,SLOT(forcePause()));
    connect(pView_,SIGNAL(goingForeground()),pScene_,SLOT(softContinue()));



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

    pScene_->restartLevel();
}

