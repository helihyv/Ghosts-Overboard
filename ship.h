#ifndef SHIP_H
#define SHIP_H

#include "orientationcontrolledgraphicspixmapobject.h"

class Ship : public OrientationControlledGraphicsPixmapObject
{
    Q_OBJECT
public:
    explicit Ship(QPixmap pixmap = 0, QGraphicsItem *parent = 0);

signals:

    /*! Emitted when a ghost is hit */
    void  pickingGhost(QGraphicsItem* pGhost);

public slots:

protected:

protected:
    bool handleCollisions();

    int ghostsAboard_;


};

#endif // SHIP_H
