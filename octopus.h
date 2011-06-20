#ifndef OCTOPUS_H
#define OCTOPUS_H

#include "timercontrolledgraphicspixmapobject.h"

class Octopus : public TimerControlledGraphicsPixmapObject
{
    Q_OBJECT
public:
    explicit Octopus(QPixmap pixmap, int speed = 10, QGraphicsItem *parent = 0);

signals:

    void droppingGhosts();

public slots:

protected:

    virtual bool handleCollisions();

};

#endif // OCTOPUS_H
