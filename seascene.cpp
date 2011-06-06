#include "seascene.h"
#include "timercontrolledtursas.h"
#include "ship.h"
#include <QGraphicsPixmapItem>
#include <QDebug>
#include <QMessageBox>

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


    //spread the rocks

    for (int i = 0; i < rocks; i++)
    {
        QPointF * pPosition = findRandomFreeSlot();

        //If there was no room no point to continue
        if (pPosition == NULL)
            break;

        QGraphicsPixmapItem * pRock = addPixmap(QPixmap(":/pix/kari.png"));
        pRock->setData(0,"rock");
        pRock->setPos(*pPosition);
        delete pPosition;

    }

    //spread the ghosts

    ghostsLeft_ = ghosts;
    spreadGhosts(ghosts);



    //spread the octopuses


    for (int i=0; i < octopuses; i++)
    {
        QPointF * pPosition = findRandomFreeSlot();

        //If there was no room no point to continue
        if (pPosition == NULL)
            break;

    TimerControlledTursas * pOctopus = new TimerControlledTursas (QPixmap(":/pix/tursas.png"),100);
    pOctopus->setData(0,"octopus");
    pOctopus->setPos(*pPosition);
    addItem(pOctopus);
    pOctopus->startMoving();
    delete pPosition;

    }


    //place the ship

    QPointF * pPosition = findRandomFreeSlot();
    if (pPosition == NULL)
    {
        // Game cannot begin without a free position for ship, so give an error message and return

        QMessageBox::critical(NULL,"Error! Too many objects on screen","No free space to place the ship. The game cannot start. Please choose another level.");
        return;
    }

    Ship * pShip = new Ship (QPixmap(":/pix/laiva.png"));
    pShip->setData(0,"ship");
    pShip->setPos(*pPosition);
    addItem(pShip);
    connect(pShip,SIGNAL(pickingGhost(QGraphicsItem*)),this, SLOT(removeGhost(QGraphicsItem*)) );
    connect(pShip,SIGNAL(droppingGhosts(int)),this,SLOT(ghostsDropped(int)));
    pShip->startMoving();
    delete pPosition;
}


void SeaScene::spreadGhosts(int ghosts)
{
    for (int i=0; i < ghosts; i++)
    {
        QPointF * pPosition = findRandomFreeSlot();

        //If there was no room no point to continue
        if (pPosition == NULL)
            return;

        QGraphicsPixmapItem * pGhost = addPixmap(QPixmap(":/pix/aave.png"));
        pGhost->setData(0,"ghost");
        pGhost->setPos(*pPosition);
        delete pPosition;
    }
}

QPointF* SeaScene::findRandomFreeSlot()
{
    if (freeTiles_.isEmpty())
        return NULL;

    int index = qrand()%freeTiles_.size();

    qDebug()  << index << " index";
    return new QPointF (freeTiles_.takeAt(index));

}

void SeaScene::removeGhost(QGraphicsItem *pGhost)
{
    removeItem(pGhost);  //remove the item from scene
    freeTiles_.append(pGhost->scenePos()); //add the item's position to free slots
    delete pGhost;
    ghostsLeft_--;
    if (ghostsLeft_ == 0)
    {
        //here whatever happens when a level is complete / a signal
        qDebug() << "All ghosts picked!";
    }
}

void SeaScene::ghostsDropped(int ghosts)
{
    ghostsLeft_ += ghosts;

    spreadGhosts(ghosts);
}
