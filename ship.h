/**************************************************************************
        Ghosts Overboard - a game for 'Meego 1.2 Harmattan'

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

#ifndef SHIP_H
#define SHIP_H

#include "orientationcontrolledgraphicspixmapobject.h"
#include <QFeedbackHapticsEffect>

class Ship : public OrientationControlledGraphicsPixmapObject
{

       Q_OBJECT
public:
    explicit Ship(QList<QPixmap> pixmapList, QGraphicsItem *parent = 0);

    virtual ~Ship();

signals:

    /*! Emitted when a ghost is hit */
    void  pickingGhost(QGraphicsItem* pGhost);



    /*! Emitted when ghosts fall over board */
    void droppingGhosts (int ghosts);

public slots:

    void dropAllGhosts();

    void setVibrationActivate(bool on);

    void allowVibration();

    void disallowVibration();

protected:
    bool handleCollisions();

    void updateShipImage();

    int ghostsAboard_;

    QList<QPixmap> shipImages_;

    bool vibrationActive_;

    bool vibrationAllowed_;

    QFeedbackHapticsEffect * pVibrationEffect_;


};

#endif // SHIP_H
