#include "timercontrolledtursas.h"
#include <QGraphicsScene>
#include <QDebug>


TimerControlledTursas::TimerControlledTursas(QPixmap pixmap, int speed, QGraphicsItem* parent) :
    QObject(), QGraphicsPixmapItem(pixmap, parent)
{
    setSpeed(speed);
    direction_ = S;
    connect(&timer_,SIGNAL(timeout()),this,SLOT(move()));
}

void TimerControlledTursas::startMoving()
{
    timer_.start();
}

void TimerControlledTursas::stopMoving()
{
    timer_.stop();
}

void TimerControlledTursas::setSpeed(int speed)
{
    timer_.setInterval(1000/speed); //converts from pixels in second to milliseconds per pixel
}

void TimerControlledTursas::move()
{

    if (!scene()) //no movement if this item does not belong to a scene
        return;

    int oldx = x();
    int oldy = y();

    int newx = oldx;
    int newy = oldy;

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


    //These three lines are identical with the orientationcontrolled version - should there be a common base class with a function to handle this?

    QRect sceneRectangle = scene()->sceneRect().toRect();
    setX(qBound(sceneRectangle.left(),newx,sceneRectangle.right()-pixmap().width()));
    setY(qBound(sceneRectangle.top(),newy,sceneRectangle.bottom()-pixmap().height()));



}


