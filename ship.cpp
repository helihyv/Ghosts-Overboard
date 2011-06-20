#include "ship.h"
#include <QDebug>

Ship::Ship(QList<QPixmap> pixmapList, QGraphicsItem *parent) :
    OrientationControlledGraphicsPixmapObject(pixmapList.at(0),parent)
{
    shipImages_ = pixmapList;
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
//        qDebug() << type;

        if (type == "rock" || type == "octopus")
        {
            // drop all ghosts when hitting an obstacle
            emit droppingGhosts(ghostsAboard_);
            ghostsAboard_ = 0;
            updateShipImage();

            return false;
        }

        else if (type == "ghost")
        {
            ghostsAboard_++;
            updateShipImage();

//            qDebug() << ghostsAboard_ << " ghosts aboard";

            emit pickingGhost(collidesList.at(0));

            return true;
        }

    }
}

void Ship::updateShipImage()
{
    int index = qBound(0,ghostsAboard_,shipImages_.length()-1);
    setPixmap(shipImages_.at(index));
}

void Ship::dropAllGhosts()
{
//TODO
}

