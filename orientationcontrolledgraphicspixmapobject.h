#ifndef ORIENTATIONCONTROLLEDGRAPHICSPIXMAPOBJECT_H
#define ORIENTATIONCONTROLLEDGRAPHICSPIXMAPOBJECT_H

#include <QGraphicsPixmapItem>
#include <QRotationSensor>

QTM_USE_NAMESPACE



class OrientationControlledGraphicsPixmapObject : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
//    explicit OrientationControlledGraphicsPixmapObject(QGraphicsItem *parent = 0);
    explicit OrientationControlledGraphicsPixmapObject(QPixmap pixmap = 0, QGraphicsItem *parent = 0);

signals:

public slots:
    void startMoving();
    void stopMoving();
    void readRotationSensor();


protected:
    /*! Returns true if the new position is to be maintained and false if a revert back to the old position is needed.
        This stub always just returns true. Actual collision handling is left for subclasses to implement (by reimplementing this function).
*/
    virtual bool handleCollisions();

private:

    QRotationSensor rotationSensor_;




};

#endif // ORIENTATIONCONTROLLEDGRAPHICSPIXMAPOBJECT_H
