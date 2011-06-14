#ifndef SHIP_H
#define SHIP_H

#include "orientationcontrolledgraphicspixmapobject.h"

class Ship : public OrientationControlledGraphicsPixmapObject
{

       Q_OBJECT
public:
    explicit Ship(QList<QPixmap> pixmapList, QGraphicsItem *parent = 0);

signals:

    /*! Emitted when a ghost is hit */
    void  pickingGhost(QGraphicsItem* pGhost);



    /*! Emitted when ghosts fall over board */
    void droppingGhosts (int ghosts);

public slots:

protected:

protected:
    bool handleCollisions();

    void updateShipImage();

    int ghostsAboard_;

    QList<QPixmap> shipImages_;


};

#endif // SHIP_H
