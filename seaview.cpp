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

SeaView::SeaView(QWidget *parent) :
    QGraphicsView(parent)
{

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
