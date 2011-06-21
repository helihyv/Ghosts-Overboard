#include "timercontrolledgraphicspixmapobject.h"
#include <QGraphicsScene>
#include <QDebug>


TimerControlledGraphicsPixmapObject::TimerControlledGraphicsPixmapObject(QPixmap pixmap, int speed, QGraphicsItem* parent) :
    QObject(), QGraphicsPixmapItem(pixmap, parent)
{
    setSpeed(speed);
    direction_ = S;
    connect(&timer_,SIGNAL(timeout()),this,SLOT(move()));
}

void TimerControlledGraphicsPixmapObject::startMoving()
{
    timer_.start();
}

void TimerControlledGraphicsPixmapObject::stopMoving()
{
    timer_.stop();
}

void TimerControlledGraphicsPixmapObject::setSpeed(int speed)
{
    timer_.setInterval(1000/speed); //converts from pixels in second to milliseconds per pixel
}

void TimerControlledGraphicsPixmapObject::move()
{

    int oldx = x();
    int oldy = y();

    int newx = oldx;
    int newy = oldy;


    //calculate the new position

    if (direction_ == E || direction_ == SE || direction_ == NE)
    {
        newx++;
    }

    if (direction_ == W || direction_ == SW || direction_ == NW)
    {
        newx--;
    }

    if (direction_ == S || direction_ == SE || direction_ == SW)
    {
        newy++;
    }

    if (direction_ == N || direction_ == NE || direction_ == NW)
    {
        newy--;
    }



    //Bound the item into the scene and change direction if hitting a boundary
    //Only works if the old position is inside the boundaries

    if (!scene()) //no movement if this item does not belong to a scene
        return;

    QRect sceneRectangle = scene()->sceneRect().toRect();

    if (newx < sceneRectangle.left() || newx > sceneRectangle.right()-pixmap().width())
    {
        changeDirection();
        return;
    }


    if (newy < sceneRectangle.top() || newy > sceneRectangle.bottom()-pixmap().height())
    {
        changeDirection();
        return;     //the old x and y values remain intact
    }


    //Set the new position

    setX(newx);
    setY(newy);


    //check for collisions and handle them (up to subclassess to implement)
    //return to the old position if requested

    if (handleCollisions() == false)
    {
        setX(oldx);
        setY(oldy);
    }


}

void TimerControlledGraphicsPixmapObject::changeDirection()
{
 //   qDebug () << "Supposed to change direction";

    int direction = (qrand()%8);
 //   qDebug()  << direction;

    switch (direction)
    {
        case 0:
            direction_ = S;
            break;

        case 1:
            direction_ = SW;
            break;

       case 2:
            direction_ = W;
            break;

       case 3:
            direction_ = NW;
            break;

       case 4:
            direction_ = N;
            break;

       case 5:
            direction_ = NE;
            break;

       case 6:
            direction_ = E;
            break;

      case 7:
            direction_ = SE;
            break;


    }

}

bool TimerControlledGraphicsPixmapObject::handleCollisions()
{
    return true;
}
