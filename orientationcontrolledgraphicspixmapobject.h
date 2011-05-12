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

    /*! sets what QGraphicsItems to treat as obstacles not to move on top of
        @param key The key to be used with QGraphicsItem::data() to find the correct value to compare
        @param values The values received from QGraphicsItem::data() to treat as obstacles
    */
    void setObstacles(int key, QList<QVariant> values);


private:

    QRotationSensor rotationSensor_;

    QRectF boundaryrect_;

    int obstacleKey_;
    QList<QVariant> obstacleValues_;

};

#endif // ORIENTATIONCONTROLLEDGRAPHICSPIXMAPOBJECT_H
