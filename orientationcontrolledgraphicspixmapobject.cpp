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


#include "orientationcontrolledgraphicspixmapobject.h"
#include <QDebug>
#include <QGraphicsScene>
#include <QPropertyAnimation>

//OrientationControlledGraphicsPixmapObject::OrientationControlledGraphicsPixmapObject (QGraphicsItem *parent) :
//    QObject(), QGraphicsPixmapItem (parent)
//{

//}

OrientationControlledGraphicsPixmapObject::OrientationControlledGraphicsPixmapObject(QPixmap pixmap, QGraphicsItem *parent) :
    QObject(), QGraphicsPixmapItem (pixmap, parent)
{

    connect(&rotationSensor_,SIGNAL(readingChanged()),this,SLOT(readRotationSensor()));

    qrangelist rangelist = rotationSensor_.availableDataRates();

    qDebug() << rangelist.length() << "ranges found";
    foreach (qrange range, rangelist)
    {
        qDebug() << "Rotation sensor: " << range.first <<", " << "range.second";
    }
}

void OrientationControlledGraphicsPixmapObject::startMoving()
{
    rotationSensor_.start();
//    qDebug() << "started the sensor";
//    qDebug() << rotationSensor_.isActive();
}


void OrientationControlledGraphicsPixmapObject::stopMoving()
{
    rotationSensor_.stop();
//    qDebug () << "trying to stop the sensor";
}

void OrientationControlledGraphicsPixmapObject::readRotationSensor()
{
    if (!scene()) //no movement if this item does not belong to a scene
        return;

    QRect sceneRectangle = scene()->sceneRect().toRect();


    QRotationReading* pSensorData = rotationSensor_.reading();

    //    int deltay = pSensorData->x(); //yes, in Maemo 5 you need the "x" value from the sensor for "y" direction in the scene...
    //    int deltax = pSensorData->y(); //...and vice versa

        int deltay = -pSensorData->y(); //But in Harmattan, you need the "y" value from the sensor for "y" direction in the scene...
        int deltax = pSensorData->x(); //...and x for x (when in landscape)
                                      //and you need to reverse the y value

 //   qDebug() << deltax << " " << deltay;

    int oldx = x();
    int oldy = y();

    //this is how it works on maemo
//    int newx = x() + deltax/15;
//    int newy = y() + deltay/15;

    //this is for Harmattan
    int newx = x() + deltax/3;
    int newy = y() + deltay/3;


//    qDebug() << sceneRectangle.left() << sceneRectangle.right();



    int finalX = qBound(sceneRectangle.left(),newx,sceneRectangle.right()-pixmap().width());
    int finalY = qBound(sceneRectangle.top(),newy,sceneRectangle.bottom()-pixmap().height());



    QPropertyAnimation * animation = new QPropertyAnimation(this,"pos",this);
    animation->setDuration(60); //milliseconds
    animation->setStartValue(pos());
    animation->setEndValue( QPointF(finalX,finalY));
    animation->start(QAbstractAnimation::DeleteWhenStopped);

    //handle collisions and move back to the original position if false returned

//    if (handleCollisions() == false)
//    {
//        setX(oldx);
//        setY(oldy);
//    }

}


bool OrientationControlledGraphicsPixmapObject::handleCollisions()
{
    return true;
}

