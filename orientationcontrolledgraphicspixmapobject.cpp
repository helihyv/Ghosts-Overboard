#include "orientationcontrolledgraphicspixmapobject.h"
#include <QDebug>
#include <QGraphicsScene>

//OrientationControlledGraphicsPixmapObject::OrientationControlledGraphicsPixmapObject (QGraphicsItem *parent) :
//    QObject(), QGraphicsPixmapItem (parent)
//{

//}

OrientationControlledGraphicsPixmapObject::OrientationControlledGraphicsPixmapObject(QPixmap pixmap, QGraphicsItem *parent) :
    QObject(), QGraphicsPixmapItem (pixmap, parent)
{

    connect(&rotationSensor_,SIGNAL(readingChanged()),this,SLOT(readRotationSensor()));



}

void OrientationControlledGraphicsPixmapObject::startMoving()
{
    rotationSensor_.start();
    qDebug() << "started the sensor";
    qDebug() << rotationSensor_.isActive();
}


void OrientationControlledGraphicsPixmapObject::stopMoving()
{
    rotationSensor_.stop();
    qDebug () << "trying to stop the sensor";
}

void OrientationControlledGraphicsPixmapObject::readRotationSensor()
{
    if (!scene()) //no movement if this item does not belong to a scene
        return;

    QRect sceneRectangle = scene()->sceneRect().toRect();


    QRotationReading* pSensorData = rotationSensor_.reading();

    int deltay = pSensorData->x(); //yes, you need the "x" value from the sensor for "y" direction in the scene...
    int deltax = pSensorData->y(); //...and vice versa


 //   qDebug() << deltax << " " << deltay;

    int oldx = x();
    int oldy = y();

    int newx = x() + deltax/15;
    int newy = y() + deltay/15;


//    qDebug() << sceneRectangle.left() << sceneRectangle.right();


    setX(qBound(sceneRectangle.left(),newx,sceneRectangle.right()-pixmap().width()));
    setY(qBound(sceneRectangle.top(),newy,sceneRectangle.bottom()-pixmap().height()));

    QList<QGraphicsItem*>  collidesList = collidingItems();
    if (!collidesList.isEmpty())
    {
        qDebug() << collidesList.at(0)->data(0);
        if (collidesList.at(0)->data(0) == "rock")
        {
            setX(oldx);
            setY(oldy);
        }
    }

}


void OrientationControlledGraphicsPixmapObject::setBoundaries(QRectF boundaryrect)
{
    boundaryrect_ = boundaryrect;

}

void OrientationControlledGraphicsPixmapObject::setObstacles(int key, QList<QVariant> values)
{
    obstacleKey_ = key;
    obstacleValues_ = values;
}



