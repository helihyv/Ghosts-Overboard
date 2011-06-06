#include "ship.h"
#include <QDebug>

Ship::Ship(QPixmap pixmap, QGraphicsItem *parent) :
    OrientationControlledGraphicsPixmapObject(pixmap,parent)
{

}

bool Ship::handleCollisions()
{
    QList<QGraphicsItem*>  collidesList = collidingItems();

    if (collidesList.isEmpty())
        return true;

    else
    {
        QString type = collidesList.at(0)->data(0).toString();
        qDebug() << type;
        if (type == "rock" || type == "octopus")
        {
//            dropGhosts();
            return false;
        }

        else if (type == "ghost")
        {
            return true;
        }

    }
}
