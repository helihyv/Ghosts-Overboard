#ifndef SEASCENE_H
#define SEASCENE_H

#include <QGraphicsScene>
#include "screenlitkeeper.h"
#include "level.h"

class SeaScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit SeaScene(QObject *parent = 0);

signals:

    void allGhostsPicked();
    void pauseOn();
    void pauseOff();
    void vibrationActivated(bool on);

public slots:

    /*! Places all needed items for a level to (re)start.
    Relies on the scene size to be explicitly set to a a value that will persist troughout play.
    @param ghosts the number of ghosts to be placed on the map
    @param rocks the number of rocks to be placed on the map
    @param octopuses number of octopuses to be placed on the map
    */
    void setupMap(int ghosts, int rocks, int octopuses, int octopusSpeed);

    void setupMap(Level level);

    void spreadGhosts(int ghosts);

    void removeGhost(QGraphicsItem * pGhost);

    void ghostsDropped(int ghosts);

    void pause (bool paused);

    void vibrationActivate(bool);

protected:

    /*! Gives a pointer to a random position if a free one is found. Otherwise returns NULL.
        The position is the upper left corner of a free 40x40 pixels slot.
    */

    QPointF* findRandomFreeSlot();

    const QString ghostImageFilename_;
    const QString rockImageFilename_;
    const QString octopusImageFilename_;
    const QStringList shipImageFilenames_;

    QList<QPointF> freeTiles_;

    int ghostsLeft_;

    QList<QGraphicsItem*> movingItems_;

    bool paused_;

    ScreenLitKeeper screenLitKeeper_;




};

#endif // SEASCENE_H
