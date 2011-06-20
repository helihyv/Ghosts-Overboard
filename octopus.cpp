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
