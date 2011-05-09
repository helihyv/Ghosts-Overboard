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
    void setBoundaries(QRectF boundaryrect);


private:

    QRotationSensor rotationSensor_;

    QRectF boundaryrect_;

};

#endif // ORIENTATIONCONTROLLEDGRAPHICSPIXMAPOBJECT_H
