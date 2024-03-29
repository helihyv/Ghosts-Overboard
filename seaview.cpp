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

#include "seaview.h"

#include <QEvent>
#include <QTimer>

SeaView::SeaView(QWidget *parent) :
    QGraphicsView(parent)
{


    setWindowTitle("Ghosts Overboard");

    pScene_ = new SeaScene ();


    setScene(pScene_);


    connect(this,SIGNAL(screenTapped()),pScene_,SLOT(handleScreenTapped()));
    connect(this,SIGNAL(goingBackgroung()),pScene_,SLOT(forcePause()));
    connect(this,SIGNAL(goingForeground()),pScene_,SLOT(softContinue()));

    connect(pScene_,SIGNAL(minimizeRequested()),this,SLOT(showNormal()));
    connect(pScene_,SIGNAL(fullscreenRequested()),this,SLOT(showFullScreen()));

    showFullScreen();



    //the boundaries of the scene are set to match the size of the view window, which is not
    //available in the constructor --> timer needed
    QTimer::singleShot(100,this,SLOT(initializeBoundaries()));

}

void  SeaView::mousePressEvent(QMouseEvent *event)
{

    QGraphicsView::mousePressEvent(event);
    emit screenTapped();


}

bool SeaView::event(QEvent *event)
{
    if (!event)
        return false;

    switch (event->type())
    {
        //pause if app goes to background
        case QEvent::WindowDeactivate:

            emit goingBackgroung();
            break;

        //un-pause if app gomes back to foreground unless it was paused before going to background
        case QEvent::WindowActivate:

            emit goingForeground();
            break;

        //Just to keep the compiler from complaining...
        default:
            break;

     }



    //pass the event to the ancestor for handling
    return QGraphicsView::event(event);

 }


void SeaView::initializeBoundaries()
{
        //the boundaries of the scene are set to match the size of the view window, and
        //the view is set to show exactly the whole scene area

    //this occasionally gives a tiny scene, so using a fixed size fit for N900/Maemo5 until a fix is found

//    QPoint topleft (0,0);
//    QSize windowsize = pView_->size();
//    QRectF rectangle (topleft,windowsize);

    QRectF rectangle(0,0,800,480);

    pScene_->setSceneRect(rectangle);
    setSceneRect(rectangle);

//     qDebug() << "Initialized boundaries" << rectangle.right() << rectangle.bottom() << pView_->width() << pView_->height();

    pScene_->restartLevel();
}

