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

#ifndef ORIENTATIONCONTROLLEDGRAPHICSPIXMAPOBJECT_H
#define ORIENTATIONCONTROLLEDGRAPHICSPIXMAPOBJECT_H

#include <QGraphicsPixmapItem>
#include <QRotationSensor>

QTM_USE_NAMESPACE



class OrientationControlledGraphicsPixmapObject : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

    Q_PROPERTY(QPointF pos READ pos WRITE setPos)

public:
//    explicit OrientationControlledGraphicsPixmapObject(QGraphicsItem *parent = 0);
    explicit OrientationControlledGraphicsPixmapObject(QPixmap pixmap = 0, QGraphicsItem *parent = 0);


signals:


public slots:
    void startMoving();
    void stopMoving();
    void readRotationSensor();
      virtual void setPos(const QPointF &pos);


protected:
    /*! Returns true if the new position is to be maintained and false if a revert back to the old position is needed.
        This stub always just returns true. Actual collision handling is left for subclasses to implement (by reimplementing this function).
*/
    virtual bool handleCollisions();


private:

    QRotationSensor rotationSensor_;
    QPointF oldOldPos_;




};

#endif // ORIENTATIONCONTROLLEDGRAPHICSPIXMAPOBJECT_H
