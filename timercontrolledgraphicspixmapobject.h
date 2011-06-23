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

#ifndef TIMERCONTROLLEDGRAPHICSPIXMAPOBJECT_H
#define TIMERCONTROLLEDGRAPHICSPIXMAPOBJECT_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QTimer>

class TimerControlledGraphicsPixmapObject : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit TimerControlledGraphicsPixmapObject(QPixmap pixmap = QPixmap(), int speed = 1, QGraphicsItem *parent = 0);


signals:

public slots:

    void startMoving();
    void stopMoving();

    /*! Intended to be used internally by connecting to a timer
      */
    void move();

    /*! Sets the movement speed of the item
      @param speed given in pixels per second
      */
    void setSpeed(int speed);


 protected:

    void changeDirection();

    virtual bool handleCollisions();

    QTimer timer_;

    enum direction {N, NE, E, SE, S, SW, W, NW};

    direction direction_;

    bool stoppedBecauseInvalidTime_;


};

#endif // TIMERCONTROLLEDGRAPHICSPIXMAPOBJECT_H
