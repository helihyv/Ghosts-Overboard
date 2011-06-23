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

#ifndef OCTOPUS_H
#define OCTOPUS_H

#include "timercontrolledgraphicspixmapobject.h"

class Octopus : public TimerControlledGraphicsPixmapObject
{
    Q_OBJECT
public:
    explicit Octopus(QPixmap pixmap, int speed = 10, QGraphicsItem *parent = 0);

signals:

    void droppingGhosts();

public slots:

protected:

    virtual bool handleCollisions();

};

#endif // OCTOPUS_H
