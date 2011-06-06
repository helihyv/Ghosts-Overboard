#include "ship.h"
#include <QDebug>

Ship::Ship(QPixmap pixmap, QGraphicsItem *parent) :
    OrientationControlledGraphicsPixmapObject(pixmap,parent)
{
    ghostsAboard_ = 0;

}

bool Ship::handleCollisions()
{
    QList<QGraphicsItem*>  collidesList = collidingItems();

    if (collidesList.isEmpty())
        return true;

    else
    {
        //since the game logic does not leave items to collide with each other we can take just the topmost one
        //and trust it is the only one
        QString type = collidesList.at(0)->data(0).toString();
        qDebug() << type;

        if (type == "rock" || type == "octopus")
        {
//            dropGhosts();
            return false;
        }

        else if (type == "ghost")
        {
            ghostsAboard_++;

            qDebug() << ghostsAboard_ << " ghosts aboard";

            emit pickingGhost(collidesList.at(0));

            return true;
        }

    }
}
