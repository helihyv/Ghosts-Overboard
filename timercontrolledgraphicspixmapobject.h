#ifndef TIMERCONTROLLEDGRAPHICSPIXMAPOBJECT_H
#define TIMERCONTROLLEDGRAPHICSPIXMAPOBJECT_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QTimer>

class TimerControlledGraphisPixmapObject : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit TimerControlledGraphicsPixmapObject(QPixmap pixmap = QPixmap(), int speed = 1, QGraphicsItem *parent = 0);


signals:

public slots:

    void startMoving();
    void stopMoving();

    /*! Intended to be used internally by connecting to a timer
      */
    void move();

    /*! Sets the movement speed of the item
      @param speed given in pixels per second
      */
    void setSpeed(int speed);


 protected:

    void changeDirection();

    virtual bool handleCollisions();

    QTimer timer_;

    enum direction {N, NE, E, SE, S, SW, W, NW};

    direction direction_;


};

#endif // TIMERCONTROLLEDGRAPHICSPIXMAPOBJECT_H
