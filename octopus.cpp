/**************************************************************************
        Ghosts Overboard 'Meego 1.2 Harmattan'

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

#include "octopus.h"

Octopus::Octopus(QPixmap pixmap, int speed, QGraphicsItem *parent) :
    TimerControlledGraphicsPixmapObject(pixmap,speed,parent)
{
}

bool Octopus::handleCollisions()
{

    QList<QGraphicsItem*>  collidesList = collidingItems();
    if (collidesList.isEmpty())
            return true; //retain new position

    else
    {
        //change direction if hit anything
        changeDirection();
        //the game allows only one object to saty at given spot, so just check the firs one
        if (collidesList.at(0)->data(0) == "ship")
        {
            emit droppingGhosts();
        }
        return false; //go back to old position
    }
}
