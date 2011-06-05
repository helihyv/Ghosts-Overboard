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

    QPointF findRandomFreeSlot();

    const QString ghostImageFilename_;
    const QString rockImageFilename_;
    const QString octopusImageFilename_;
    const QStringList shipImageFilenames_;

    QList<QPointF> freeTiles_;




};

#endif // SEASCENE_H
