#include "seascene.h"
#include <QGraphicsPixmapItem>
#include <QDebug>

const QString ghostImageFilename_ = ":/pix/aave.png";
const QString rockImageFilename_ =":/pix/kari.png";
const QString octopusImageFilename_= ":/pix/tursas.png";


SeaScene::SeaScene(QObject *parent) :
    QGraphicsScene(parent)
{


}

void SeaScene::setupMap(int ghosts, int rocks, int octopuses)
{
    //empty the list of free slots
    freeTiles_.clear();

    //fill the list of free slots

    int numberOfXTiles  = width() / 40;
    int numberOfYTiles = height() /40;

    qDebug() << numberOfXTiles << " slots in x direction";
    qDebug() << numberOfYTiles << " slots in y rirection";

    for (int i = 0; i < numberOfXTiles; i++ )
    {
        for (int j = 0; j < numberOfYTiles; j++)
        {
            freeTiles_.append(QPointF(i*40,j*40));
        }
    }
    spreadGhosts(ghosts);
}

void SeaScene::spreadGhosts(int ghosts)
{
    for (int i=0; i < ghosts; i++)
    {
        QPointF position = findRandomFreeSlot();

        QGraphicsPixmapItem * pGhost = addPixmap(QPixmap(":/pix/aave.png"));
        pGhost->setData(0,"ghost");
        pGhost->setPos(position);
    }
}

QPointF SeaScene::findRandomFreeSlot()
{
    int index = qrand()%freeTiles_.size();

    qDebug()  << index << " index";
    return freeTiles_.takeAt(index);

}
