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
#include <QApplication>
#include <QAction>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>

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

//Setup the level list

    Level level1(5,10);
    levelList_.append(level1);
    Level level2(5,10,2,50);
    levelList_.append(level2);
    Level level3(5,15,2,50);
    levelList_.append(level3);
    Level level4(5,15,4,50);
    levelList_.append(level4);
    Level level5(5,15,5,100);
    levelList_.append(level5);

    currentLevel_ = 0;

    connect(this,SIGNAL(allGhostsPicked()),this,SLOT(nextLevel()));


    pVibrateAction_ = new QAction(tr("Vibration effects"),this);
    pVibrateAction_->setCheckable(true);
    connect(pVibrateAction_,SIGNAL(toggled(bool)),this,SLOT(vibrationActivate(bool)));


    pPauseAction_ = new QAction(tr("Pause"),this);
    pPauseAction_->setCheckable(true);
    connect(pPauseAction_,SIGNAL(toggled(bool)),this,SLOT(pause(bool)));


}

void SeaScene::setupMap(int ghosts, int rocks, int octopuses, int octopusSpeed)
{
    //empty the map

    clear();

    createMenuItems();

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
            if (pPausetextItem_)
                pPausetextItem_->hide();
        }

        else
        {
         qDebug("about to stop movement");
            emit pauseOn();
            screenLitKeeper_.keepScreenLit(false);
            if (pPausetextItem_ != NULL)
            {
                qDebug() << "about to show the pause text";
                pPausetextItem_->show();
                qDebug() << "showing pause text";
            }
                else qDebug() << "No pause text available";
        }
}

void SeaScene::vibrationActivate(bool on)
{
    emit vibrationActivated(on);
}

void SeaScene::handleScreenTapped()
{

    //If the game is going just pause it
    if (!paused_)
    {
        pPauseAction_->setChecked(true);
        return;
    }

    //If the game is paused, chacl if menu item was selected

    QList<QGraphicsItem *> items = selectedItems();

    //if nothing selected resume play

    if (items.isEmpty())
    {
        pPauseAction_->setChecked(false);
        return;

    }

    //If something was selected check if it was one of the menu items and act on it
    //(Nothing else should be made selectable anyway)

    //Menu functions

    QGraphicsItem* pItem = items.at(0); //Selecting an item brings here, thus only selecting one item should be possible
                                       //... so we can just take the first one


    if (pItem == pRestartGameItem_)
    {
        qDebug() << "game restart requested";
        restartGame();
    }

    else if (pItem == pRestartLevelItem_)
    {
        qDebug() << "Level restart requested";
        restartLevel();

    }

    else if (pItem == pSettingsItem_)
    {
    //Temporary code for settings, likely to be turned into a QML dialog

          QMessageBox::StandardButton buttonpressed = QMessageBox::question(NULL,"Settings","Do you wish to have vibration effects enabled?", QMessageBox::Yes | QMessageBox::No);

          if (buttonpressed == QMessageBox::Yes)
              pVibrateAction_->setChecked(true);
          if (buttonpressed == QMessageBox::No)
              pVibrateAction_->setChecked(false);
    }

    else if (pItem == pAboutItem_)
    {
        about();
    }


    //Selection is just used to get notice of a menu item being clicked, removed after use

    clearSelection();

}



void SeaScene::createMenuItems()
{

    pPausetextItem_ =  addSimpleText("Game paused. Tap to continue.");
    pPausetextItem_->setZValue(1000);
    pPausetextItem_->setPos(250,50);
    pPausetextItem_->hide();

    menuItemCount_ = 0;

    pRestartGameItem_ = new QGraphicsSimpleTextItem("Restart game");
    prepareForMenu(pRestartGameItem_);

    pRestartLevelItem_ = new QGraphicsSimpleTextItem("Restart level");
    prepareForMenu(pRestartLevelItem_);

    pSettingsItem_ = new QGraphicsSimpleTextItem("Settings");
    prepareForMenu(pSettingsItem_);

    pAboutItem_ = new QGraphicsSimpleTextItem("About");
    prepareForMenu(pAboutItem_);

}

void SeaScene::prepareForMenu(QGraphicsItem * pItem)
{

    //Menu items have pause text item as their parent and are thus added to scene automatically
    //They are also shown and hidden with it, resulting in the menu being visble when the game is paused
    //Their coordinates are given relative to the parent.

    pItem->setParentItem(pPausetextItem_);
    pItem->setZValue(1000);
    pItem->setFlag(QGraphicsItem::ItemIsSelectable);
    pItem->setY(150);
    pItem->setX(menuItemCount_++*150-250);
 }


void SeaScene::about()
{
    QMessageBox::about(NULL, tr("About %1").arg(QApplication::applicationName()),
                       tr("Version %1"
                          "<p>Copyright 2011 Heli Hyv&auml;ttinen"
                          "<p>License: General Public License v2"
                          "<p>Bug Reports: https://bugs.maemo.org/ "
                          "enter_bug.cgi?product=Ghosts%20Overboard"
                          ).arg(QApplication::applicationVersion()));



}


void SeaScene::restartLevel()
{
    setupMap(levelList_.value(currentLevel_));  //value() returns default constructor Level if index is invalid, so no risk of crash
    vibrationActivate(pVibrateAction_->isChecked());  //Vibration effects are lost without this
   // qDebug() << pVibrateAction_->isChecked();
}



void SeaScene::nextLevel()
{

    currentLevel_++;

    if (levelList_.empty())
        setupMap(Level());


    if ( currentLevel_ < levelList_.size() )
    {
       restartLevel();
    }

    else //Victory!
    {

       QDialog* pVictoryDialog = new QDialog();
       pVictoryDialog->setWindowTitle(tr("You won!"));


       QPushButton* pPlayAgainButton = new QPushButton(tr("Play again"));
//       QPushButton* pQuitButton = new QPushButton(tr("Quit game"));

       QPixmap victoryIcon (":/pix/aavesaari.png");
       QLabel* pVictoryLabel = new QLabel();
       pVictoryLabel->setPixmap(victoryIcon);

       QLabel* pTextLabel = new QLabel(tr("Congratulations! <p>You have saved all the ghosts."));


       QVBoxLayout* pMainLayout = new QVBoxLayout;

       QHBoxLayout* pTopLayout = new QHBoxLayout;
       pMainLayout->addLayout(pTopLayout);

       pTopLayout->addWidget(pVictoryLabel);
       pTopLayout->addWidget(pTextLabel);



       QHBoxLayout* pButtonLayout = new QHBoxLayout();
       pMainLayout->addLayout(pButtonLayout);

 //      pButtonLayout->addWidget(pQuitButton);
       pButtonLayout->addWidget(pPlayAgainButton);



       pVictoryDialog->setLayout(pMainLayout);

       connect(pPlayAgainButton, SIGNAL(clicked()),pVictoryDialog,SLOT(accept()));

       pVictoryDialog->exec();

        //Never mind if the user cancels the dialog: restart the game anyway

       restartGame();
    }
}


void SeaScene::restartGame()
{
    currentLevel_ = 0;
    restartLevel();
}


void SeaScene::forcePause()
{
    //Pause without setting the pause action state
    pause(true);
}

void::SeaScene::softContinue()
{
    //Continue if not being paused by the user
    // Reverts forcePause()

    pause(pPauseAction_->isChecked());
}
