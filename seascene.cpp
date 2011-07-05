/**************************************************************************
        Ghosts Overboard - a game for Maemo 5

        Copyright (C) 2011  Heli Hyvättinen

        This file is part of Ghosts Overboard

        Ghosts Overboard is free software: you can redistribute it and/or modify
        it under the terms of the GNU General Public License as published by
        the Free Software Foundation, either version 2 of the License, or
        (at your option) any later version.

        This program is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
        GNU General Public License for more details.

        You should have received a copy of the GNU General Public License
        along with this program.  If not, see <http://www.gnu.org/licenses/>.

**************************************************************************/

#include "seascene.h"
#include "octopus.h"
#include "ship.h"
#include <QGraphicsPixmapItem>
#include <QDebug>
#include <QMessageBox>
#include <QTime>

const QString ghostImageFilename_ = ":/pix/aave.png";
const QString rockImageFilename_ =":/pix/kari.png";
const QString octopusImageFilename_= ":/pix/tursas.png";


SeaScene::SeaScene(QObject *parent) :
    QGraphicsScene(parent)
{
    paused_ = false;
    screenLitKeeper_.keepScreenLit(true);

    //set background

    QPixmap waves (":/pix/meri.png");
    waves.scaled(20,20);
    setBackgroundBrush(QBrush(waves));

    //set random seed

    qsrand(QTime::currentTime().msec()+2);  //+2 to avoid setting it to 1



}

void SeaScene::setupMap(int ghosts, int rocks, int octopuses, int octopusSpeed)
{
    //empty the map

    clear();

    //empty the list of moving items

    movingItems_.clear();

    //empty the list of free slots
    freeTiles_.clear();

    //fill the list of free slots

    int numberOfXTiles  = width() / 40;
    int numberOfYTiles = height() /40;

//    qDebug() << numberOfXTiles << " slots in x direction";
//    qDebug() << numberOfYTiles << " slots in y rirection";

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

        QPixmap rockPixmap (":/pix/kari.png");
        QGraphicsPixmapItem * pRock = addPixmap(rockPixmap);
        pRock->setData(0,"rock");
        pRock->setPos(*pPosition);
        delete pPosition;

    }

    //spread the ghosts

    ghostsLeft_ = ghosts;
    spreadGhosts(ghosts);



    //spread the octopuses

    QList <Octopus*> octopusList;

    for (int i=0; i < octopuses; i++)
    {
        QPointF * pPosition = findRandomFreeSlot();

        //If there was no room no point to continue
        if (pPosition == NULL)
            break;

    QPixmap octopusPixmap (":/pix/tursas.png");
    Octopus * pOctopus = new Octopus(octopusPixmap,octopusSpeed);
    pOctopus->setData(0,"octopus");
    pOctopus->setPos(*pPosition);
    addItem(pOctopus);
    pOctopus->startMoving();
    movingItems_.append(pOctopus);
    connect(this,SIGNAL(pauseOn()),pOctopus,SLOT(stopMoving()));
    connect(this,SIGNAL(pauseOff()),pOctopus,SLOT(startMoving()));
    octopusList.append(pOctopus);
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

    QList<QPixmap> shipImages;
    shipImages.append(QPixmap(":/pix/laiva.png"));
    shipImages.append(QPixmap(":/pix/laiva_1aave.png"));
    shipImages.append(QPixmap(":/pix/laiva_2aave.png"));
    shipImages.append(QPixmap(":/pix/laiva_3aave.png"));
    shipImages.append(QPixmap(":/pix/laiva_4aave.png"));
    shipImages.append(QPixmap(":/pix/laiva_5aave.png"));
    shipImages.append(QPixmap(":/pix/laiva_6aave.png"));
    shipImages.append(QPixmap(":/pix/laiva_7aave.png"));
    shipImages.append(QPixmap(":/pix/laiva_8aave.png"));
    shipImages.append(QPixmap(":/pix/laiva_9aave.png"));
    shipImages.append(QPixmap(":/pix/laiva_10aave.png"));

    Ship * pShip = new Ship (shipImages);
    pShip->setData(0,"ship");
    pShip->setPos(*pPosition);
    addItem(pShip);
    connect(pShip,SIGNAL(pickingGhost(QGraphicsItem*)),this, SLOT(removeGhost(QGraphicsItem*)) );
    connect(pShip,SIGNAL(droppingGhosts(int)),this,SLOT(ghostsDropped(int)));
    connect(this,SIGNAL(vibrationActivated(bool)),pShip,SLOT(setVibrationActivate(bool)));
    pShip->startMoving();
    movingItems_.append(pShip);
    connect(this,SIGNAL(pauseOn()),pShip,SLOT(stopMoving()));
    connect(this,SIGNAL(pauseOff()),pShip,SLOT(startMoving()));
    foreach (Octopus* pOctopus, octopusList)
    {
        connect(pOctopus,SIGNAL(droppingGhosts()),pShip,SLOT(dropAllGhosts()));
    }
    delete pPosition;
}

void SeaScene::setupMap(Level level)
{
    setupMap(level.getNumberOfGhosts(),level.getNumberOfRocks(),level.getNumberOfOctopuses(),level.getOctopusSpeed());
}

void SeaScene::spreadGhosts(int ghosts)
{


    //the octopuses and the ship may have moved from their original positions,
    //so the list of free slots must be adjusted to exclude their current positions

    QList<QPointF> temporarilyReservedSlots;

    foreach (QGraphicsItem* pItem, movingItems_)
    {
        if (pItem == NULL)
        {
 //           qDebug() << "NULL item in movingItems_";
            continue;
        }

        //round x and y down to fit the slot size
        int x = pItem->x();
        x = x/40;
        x = x*40;

        int y = pItem->y();
        y = y/40;
        y=y*40;


        QPointF position (x,y);

        //remove the tiles (potentially) occupied by the item from free slots and place in temp list if was in the list before

        if (freeTiles_.removeOne(position))
            temporarilyReservedSlots.append(position);


        position.setX(x+40);

        if (freeTiles_.removeOne(position))
            temporarilyReservedSlots.append(position);

        position.setY(y+40);

        if (freeTiles_.removeOne(position))
            temporarilyReservedSlots.append(position);

        position.setX(x);

        if (freeTiles_.removeOne(position))
            temporarilyReservedSlots.append(position);

    }


    //spread ghosts in random free slots

    for (int i=0; i < ghosts; i++)
    {
        QPointF * pPosition = findRandomFreeSlot();

        //If there was no room no point to continue
        if (pPosition == NULL)
            return;

        QPixmap ghostPixmap(":/pix/aave.png");
        QGraphicsPixmapItem * pGhost = addPixmap(ghostPixmap);
        pGhost->setData(0,"ghost");
        pGhost->setPos(*pPosition);
        delete pPosition;
    }

    //return the slots occupied by moving items to free slots
    freeTiles_.append(temporarilyReservedSlots);

    //clear temp for the next round
    temporarilyReservedSlots.clear();
}

QPointF* SeaScene::findRandomFreeSlot()
{
    if (freeTiles_.isEmpty())
        return NULL;

    int index = qrand()%freeTiles_.size();

//    qDebug()  << index << " index";
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
        emit allGhostsPicked();
 //       qDebug() << "All ghosts picked!";
    }
}

void SeaScene::ghostsDropped(int ghosts)
{
    ghostsLeft_ += ghosts;

    spreadGhosts(ghosts);
}

void SeaScene::pause(bool paused)
{
    //    qDebug() << "pause pressed " << paused;
        if (paused_ == paused)
                return;

        paused_ = paused;

        if (paused == false)
        {
     //       qDebug() << "starting to move again";
            emit pauseOff();
            screenLitKeeper_.keepScreenLit(true);
        }

        else
        {
     //       qDebug("about to stop movement");
            emit pauseOn();
            screenLitKeeper_.keepScreenLit(false);
        }
}

void SeaScene::vibrationActivate(bool on)
{
    emit vibrationActivated(on);
}
