#ifndef SEASCENE_H
#define SEASCENE_H

#include <QGraphicsScene>

class SeaScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit SeaScene(QObject *parent = 0);

signals:

public slots:

    void setupMap(int ghosts, int rocks, int octopuses);

    void spreadGhosts(int ghosts);

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




};

#endif // SEASCENE_H
