/**************************************************************************
        Ghosts Overboard - a game for 'Meego 1.2 Harmattan'

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
#include <QSettings>
#include <QPixmap>


const QString ghostImageFilename_ = ":/pix/aave.png";
const QString rockImageFilename_ =":/pix/kari.png";
const QString octopusImageFilename_= ":/pix/tursas.png";


SeaScene::SeaScene(QObject *parent) :
    QGraphicsScene(parent)
{

    setItemPointersNull();

    paused_ = false;
    screenLitKeeper_.keepScreenLit(true);

    //set background

    QPixmap waves (":/pix/meri.png");
    waves.scaled(20,20);
    setBackgroundBrush(QBrush(waves));

    //set random seed

    qsrand(QTime::currentTime().msec()+2);  //+2 to avoid setting it to 1



//Setup the level list

    QList<Level> levelList;
    Level level1(5,10);
    levelList.append(level1);
    Level level2(5,10,2,50);
    levelList.append(level2);
    Level level3(5,15,2,50);
    levelList.append(level3);
    Level level4(5,15,4,50);
    levelList.append(level4);
    Level level5(5,15,5,100);
    levelList.append(level5);

    Levelset set ("Original",levelList);
    levelset_ = set;
    availableLevelsets_.append(set);

    currentLevel_ = 0;

    totalScore_ = 0;

   //Create another set of levels and place it in the available levelsets list
    levelList.clear();
    Level set2level1(8,15,4,50);
    levelList.append(set2level1);
    Level set2level2(8,20,4,50);
    levelList.append(set2level2);
    Level set2level3(8,20,5,80);
    levelList.append(set2level3);
    Level set2level4(8,20,6,120);
    levelList.append(set2level4);
    Level set2level5(8,25,8,150);
    levelList.append(set2level5);

    Levelset set2("Difficult",levelList);
    availableLevelsets_.append(set2);


    //This ensures that nextlevel will not be called until its safe to delete the Ship object.
    //Leaving out Qt::QueuedConnection or calling nextlevel directly instead of emitting the signal will CRASH
    connect(this,SIGNAL(allGhostsPicked()),this,SLOT(nextLevel()),Qt::QueuedConnection);




    pVibrateAction_ = new QAction(tr("Vibration effects"),this);
    pVibrateAction_->setCheckable(true);
    connect(pVibrateAction_,SIGNAL(toggled(bool)),this,SLOT(vibrationActivate(bool)));
    QSettings settings;
    pVibrateAction_->setChecked(settings.value("vibration",false).toBool());


    pPauseAction_ = new QAction(tr("Pause"),this);
    pPauseAction_->setCheckable(true);
    connect(pPauseAction_,SIGNAL(toggled(bool)),this,SLOT(pause(bool)));


    autopauseTimer.setSingleShot(true);
    autopauseTimer.setInterval(15*60*1000);
    connect(&autopauseTimer,SIGNAL(timeout()),this,SLOT(turnPauseOn()));

    vibrationAllowed_ = false;
    pResourceSet_ = new ResourcePolicy::ResourceSet("game",this);
    ResourcePolicy::VibraResource * pVibraResource = new ResourcePolicy::VibraResource();
    pVibraResource->setOptional(false); //The only resource of the set, so no sense for it to be optional
    pResourceSet_->addResourceObject(pVibraResource);

    connect(pResourceSet_,SIGNAL(resourcesGranted(const QList< ResourcePolicy::ResourceType >)),this,SLOT(resourcesAvailable()));
    connect(pResourceSet_,SIGNAL(lostResources()),this,SLOT(resourcesLost()));

    //To test whether resources were succesfully asked but not given
    connect(pResourceSet_,SIGNAL(resourcesDenied()),this,SLOT(resourcesLost()));
    pResourceSet_->setAlwaysReply();

    pResourceSet_->acquire();

}

void SeaScene::setupMap(int ghosts, int rocks, int octopuses, int octopusSpeed)
{
    //empty the map

    clear();

    setItemPointersNull();

    createMenuItems();

    createAboutBoxItems();

    createSelectLevelsetFromListItems();

    createVictoryItems();

    createLevelCompletedItems();


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
    if (vibrationAllowed_)
        pShip->allowVibration(); //Vibration is disallowed by default so only allowing needs to be done explicitly
    connect(pResourceSet_,SIGNAL(resourcesGranted(const QList< ResourcePolicy::ResourceType >)),pShip,SLOT(allowVibration()));
    connect(pResourceSet_,SIGNAL(lostResources()),pShip,SLOT(disallowVibration()));
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
//            emit fullscreenRequested();   fremantle specific (since no "show statusbar" action in harmattan version)
            emit pauseOff();
            screenLitKeeper_.keepScreenLit(true);
            if (pPausetextItem_)
                pPausetextItem_->hide();

            scoreCounter_.start();

            autopauseTimer.start(); //Start counting towards autopause
        }

        else
        {
//         qDebug("about to stop movement");
            emit pauseOn();
            screenLitKeeper_.keepScreenLit(false);
            if (pPausetextItem_ != NULL)
            {
//                qDebug() << "about to show the pause text";
                pPausetextItem_->show();
//                qDebug() << "showing pause text";
            }
//                else qDebug() << "No pause text available";

            levelScore_ += scoreCounter_.elapsed();

            autopauseTimer.stop(); //No need to count toward autopause when already paused
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

    //If the game is paused and about box is shown, close it and show the pause text and menu again

    if(pAboutBoxItem_)
    {
        if(pAboutBoxItem_->isVisible())
        {
            pAboutBoxItem_->hide();
            pPausetextItem_->show();
            return;
        }
    }

    //If the game is paused, check if the level completed item is shown

    if (pLevelCompletedItem_)
    {
        if (pLevelCompletedItem_->isVisible())
        {
            pLevelCompletedItem_->hide();
            restartLevel(); //Current level has already been set to the next one before showing the level completed item
            pPauseAction_->setChecked(false); //unpause
            return;
        }
    }
   
    //If the game is paused, check if the victory item is being shown
    if(pVictoryCongratulationsItem_)
    {
        if (pVictoryCongratulationsItem_->isVisibleTo(NULL)) //returns visibility to scene
        {
            pVictoryCongratulationsItem_->hide();
            restartGame();
            pPauseAction_->setChecked(false); // unpause
            return;
        }
    }


    //If the game is paused and no victory or about box, check if menu item was selected

    QList<QGraphicsItem *> items = selectedItems();

    //if nothing selected resume play

    if (items.isEmpty())
    {
        pSelectLevelsetFromListItem_->hide();
        pPauseAction_->setChecked(false);

        return;

    }

    //If something was selected check if it was one of the menu items and act on it
    //(Nothing else should be made selectable anyway)

    //Menu functions

    QGraphicsItem* pItem = items.at(0); //Selecting an item brings here, thus only selecting one item should be possible
                                       //... so we can just take the first one

    //Selection is just used to get notice of a menu item being clicked, removed after use

    clearSelection();


    //Act upon the selected item


    if (pItem == pRestartGameItem_)
    {
//        qDebug() << "game restart requested";
        restartGame();
        pPauseAction_->setChecked(false); //unpause game

    }

    else if (pItem == pRestartLevelItem_)
    {
//        qDebug() << "Level restart requested";
        restartLevel();
        pPauseAction_->setChecked(false); //unpause game

    }

    else if (pItem == pSettingsItem_)
    {
        pVibrateAction_->toggle();

        QSettings settings;
        settings.setValue("vibration",pVibrateAction_->isChecked());

        QString text = pSettingsItem_->toHtml();
        if (pVibrateAction_->isChecked())
            text.replace(" on"," off"); //don't remove spaces or you get vibratioff...
        else
            text.replace(" off"," on");
        pSettingsItem_->setHtml(text);
    }

    else if (pItem == pAboutItem_)
    {
        about();
    }

    else if (pItem == pQuitItem_)
    {
        qApp->quit();
    }

    else if (pItem == pChooseLevelsetItem_)
    {
        pPausetextItem_->hide();
        pSelectLevelsetFromListItem_->show();
    }

    else
    {
        foreach (QGraphicsItem* pLevelItem, levelsetItems_)
        {
            if (pItem == pLevelItem)
            {
                QVariant variant = pLevelItem->data(0);

                if (variant.canConvert<Levelset>())
                {
                    levelset_ = variant.value<Levelset>();
                    restartGame();
                    pPauseAction_->setChecked(false); //unpause game
                }
            }
        }
    }

}



void SeaScene::createMenuItems()
{

    QFont font;
    font.setPixelSize(35);



    pPausetextItem_ = new QGraphicsTextItem;

 // This is for fremantle
//    pPausetextItem_->setHtml("<font size = \"5\" color = darkorange> Game paused. Tap to continue.");

//Harmattan needs bigger font
    pPausetextItem_->setHtml("<font size = \"7\" color = darkorange> Game paused. Tap to continue.");


    pPausetextItem_->setZValue(1000);
    pPausetextItem_->setPos(200,50);
    addItem(pPausetextItem_);
    pPausetextItem_->hide();

    menuItemCount_ = 0;

  // This is for fremantle
 //   QString menufonthtml = "<font size = \"4\" color = darkorange>";


 //Harmattan needs bigger font
        QString menufonthtml = "<font size = \"6\" color = darkorange>";


    pRestartGameItem_ = new QGraphicsTextItem;
    pRestartGameItem_->setHtml(tr("Restart <br> game").prepend(menufonthtml));
    prepareForMenu(pRestartGameItem_);

    pRestartLevelItem_ = new QGraphicsTextItem;
    pRestartLevelItem_->setHtml(tr("Restart <br> level").prepend(menufonthtml));
    prepareForMenu(pRestartLevelItem_);

    pChooseLevelsetItem_ = new QGraphicsTextItem;
    pChooseLevelsetItem_->setHtml(tr("Choose <br> levelset").prepend(menufonthtml));
    prepareForMenu(pChooseLevelsetItem_);

    pSettingsItem_ = new QGraphicsTextItem;
    QString vibraText(tr("Turn vibration <br> effects "));
    QString statusText;
    if (pVibrateAction_->isChecked())
    {
        statusText = "off";
    }
    else
    {
        statusText = "on";
    }
    vibraText.append(statusText);
    pSettingsItem_->setHtml(vibraText.prepend(menufonthtml));
    prepareForMenu(pSettingsItem_);

    pAboutItem_ = new QGraphicsTextItem;
    pAboutItem_->setHtml(tr("About <br> game").prepend(menufonthtml));
    prepareForMenu(pAboutItem_);

    pQuitItem_ = new QGraphicsTextItem;
    pQuitItem_->setHtml(tr("Quit <br> game").prepend(menufonthtml));
    prepareForMenu(pQuitItem_);

}

void SeaScene::prepareForMenu(QGraphicsItem * pItem)
{

    //Menu items have pause text item as their parent and are thus added to scene automatically
    //They are also shown and hidden with it, resulting in the menu being visble when the game is paused
    //Their coordinates are given relative to the parent.

    int itemsPerRow = 3;

    pItem->setParentItem(pPausetextItem_);
    pItem->setZValue(1000);
    pItem->setFlag(QGraphicsItem::ItemIsSelectable);

    if (menuItemCount_< itemsPerRow)
    {
        pItem->setY(120);
    }
    else pItem->setY(240);


    pItem->setX((menuItemCount_++%itemsPerRow)*180-10);
 }


void SeaScene::about()
{
    pPausetextItem_->hide();
    pAboutBoxItem_->show();
}


void SeaScene::restartLevel()
{

    levelScore_ = 0;

    setupMap(levelset_.getLevel(currentLevel_));  //getLevel() returns default constructor Level if index is invalid, so no risk of crash

    scoreCounter_.start();

    vibrationActivate(pVibrateAction_->isChecked());  //Vibration effects are lost without this
   // qDebug() << pVibrateAction_->isChecked();

    autopauseTimer.start();  //reset counting towards autopause


}



void SeaScene::nextLevel()
{

    //get score for previous level
    levelScore_ += scoreCounter_.elapsed();
    totalScore_ += levelScore_;
    int highscore = levelset_.getLevelHighScore(currentLevel_);

    qDebug() << highscore;

    QString scoretext;

    if (levelScore_ >= highscore)
    {
        scoretext = tr("<font size=\"7\" color = darkorange>Your time: %1.%2 s<br>Best time: %3.%4 s").arg(levelScore_/1000).arg((levelScore_%1000)/100).arg(highscore/1000).arg((highscore%1000)/100);
    }

    else //New high score!

    {
        scoretext = tr("<font size=\"7\" color = darkorange>Your time %1.%2 s is<br>the new best time!").arg(levelScore_/1000).arg((levelScore_%1000)/100);
        levelset_.setLevelHighScore(currentLevel_,levelScore_);
    }

    //pause to show the highscore or victory screen

    turnPauseOn();
    pPausetextItem_->hide();


    //Go to the next level if available
    currentLevel_++;

    if ( currentLevel_ < levelset_.numberOfLevels() )
    {

       pLevelCompletedItem_->setHtml(scoretext);
       pLevelCompletedItem_->show();
//       restartLevel();
    }

    else //Victory!
    {
        int totalHighsore = levelset_.getTotalHighScore();
        if (totalScore_ >= totalHighsore)
        {
            scoretext.append(tr("<br>Your total time: %1.%2 s<br>Best total time: %3.%4 s").arg(totalScore_/1000).arg((totalScore_%1000)/100).arg(totalHighsore/1000).arg((totalHighsore%1000)/100));
        }
        else //new total high score
        {
            scoretext.append(tr("<br>Your total time %1.%2 s is<br>the new best time").arg(totalScore_/1000).arg((totalScore_%1000)/100));
            levelset_.setTotalHighScore(totalScore_);
        }

        pVictoryScoreItem_->setHtml(scoretext);
        pVictoryCongratulationsItem_->show();
    }
}


void SeaScene::restartGame()
{
    currentLevel_ = 0;
    totalScore_ = 0;
    restartLevel();
}


void SeaScene::forcePause()
{
    //Pause without setting the pause action state
    pause(true);
}

void SeaScene::softContinue()
{
    //Continue if not being paused by the user
    // Reverts forcePause()

    pause(pPauseAction_->isChecked());
}


void SeaScene::createVictoryItems()
{
    pVictoryCongratulationsItem_ = new QGraphicsTextItem;
    pVictoryCongratulationsItem_->setHtml("<font size=\"7\" color = darkorange> <b> Victory!");
    pVictoryCongratulationsItem_->hide();
    pVictoryCongratulationsItem_->setPos(315,30);
    pVictoryCongratulationsItem_->setZValue(1000);
    addItem(pVictoryCongratulationsItem_);


    //coordinates are relative to the parent

    QGraphicsTextItem * pTextItem = new QGraphicsTextItem(pVictoryCongratulationsItem_);
    pTextItem->setHtml("<font size=\"7\" color = darkorange> Congratulations!");
    pTextItem->setPos(-50,80);
    pTextItem->setZValue(1000);

    QGraphicsTextItem * pMiddleTextItem = new QGraphicsTextItem(pVictoryCongratulationsItem_);
    pMiddleTextItem->setHtml("<font size=\"7\" color = darkorange> You have saved all the ghosts.");
    pMiddleTextItem->setPos(-145,120);
    pMiddleTextItem->setZValue(1000);


    pVictoryScoreItem_ = new QGraphicsTextItem(pVictoryCongratulationsItem_);
    pVictoryScoreItem_->setPos(-50,180);
    pMiddleTextItem->setZValue(1000);
    //Text is set at usetime!

    QGraphicsTextItem * pLowestTextItem = new QGraphicsTextItem(pVictoryCongratulationsItem_);
    pLowestTextItem->setHtml("<font size=\"7\" color = darkorange> Tap to play again");
    pLowestTextItem->setPos(-50,360);
    pLowestTextItem->setZValue(1000);
}


void SeaScene::createAboutBoxItems()
{
    pAboutBoxItem_ = new QGraphicsTextItem;
    addItem(pAboutBoxItem_);
    pAboutBoxItem_->setPos(25,50);
    pAboutBoxItem_->setZValue(1000);
    pAboutBoxItem_->hide();

    pAboutBoxItem_->setHtml(tr("<font color = darkorange size = \"7\"><b>"
                          "%1 Version %2</b>"
                          "<p><font size = \"6\"> Copyright 2011 Heli Hyv&auml;ttinen"
                          "<p><font size = \"6\"> License: GNU General Public License v2"
                          "<p><font size = \"6\">  The source code of this game is available trough it's web pages."
                          "<p><font size = \"6\"> Web: http://ghostsoverboard.garage.maemo.org/<br>"
                          "Bug Reports: <br> https://bugs.maemo.org/"
                          "enter_bug.cgi?product=Ghosts%20Overboard"
                          ).arg(QApplication::applicationName(),QApplication::applicationVersion()));

}

void SeaScene::setItemPointersNull()
{
    pPausetextItem_ = NULL;
    pRestartLevelItem_ = NULL;
    pRestartGameItem_ = NULL;
    pSettingsItem_ = NULL;
    pAboutItem_ = NULL;
    pQuitItem_ = NULL ;
//    pMinimizeItem_ = NULL; //Fremantle spesific
    pChooseLevelsetItem_ = NULL;

    pAboutBoxItem_ = NULL;
    pLevelCompletedItem_ = NULL;
    pVictoryCongratulationsItem_ = NULL;
    pVictoryScoreItem_ = NULL;

}

void SeaScene::turnPauseOn()
{
    pPauseAction_->setChecked(true);
}



void SeaScene::createLevelCompletedItems()
{
    pLevelCompletedItem_ = new QGraphicsTextItem;
    addItem(pLevelCompletedItem_);
    pLevelCompletedItem_->setPos(240,100);
    pLevelCompletedItem_->setZValue(1000);
    pLevelCompletedItem_->hide();
    //The text is set at usetime

    QGraphicsTextItem * pTapForNextLevelItem = new QGraphicsTextItem(pLevelCompletedItem_);
    pTapForNextLevelItem->setPos(-60,100);
    pTapForNextLevelItem->setZValue(1000);
    pTapForNextLevelItem->setHtml("<font size=\"7\" color = darkorange>Tap to start the next level");
}

void SeaScene::createSelectLevelsetFromListItems()
{

    if (availableLevelsets_.isEmpty()) //Something is badly wrong in code if this is true...
            return;


    pSelectLevelsetFromListItem_ = new QGraphicsTextItem;
    addItem(pSelectLevelsetFromListItem_);
    pSelectLevelsetFromListItem_->setPos(295,60);
    pSelectLevelsetFromListItem_->setZValue(1000);
    pSelectLevelsetFromListItem_->hide();


    QString fontstring ("<font color = darkorange size = \"7\">");

    pSelectLevelsetFromListItem_->setHtml(tr("Choose a levelset").prepend(fontstring));

    int yPos = 100;

    levelsetItems_.clear();


    foreach (Levelset set, availableLevelsets_)
    {
        QGraphicsTextItem * pItem = new QGraphicsTextItem(pSelectLevelsetFromListItem_);
        QString text (fontstring);
        if (levelset_.getName() == set.getName())
            text.append("<b>");
        text.append(set.getName());
        pItem->setHtml(text);
        pItem->setPos(65,yPos);
        yPos+=80;
        pItem->setZValue(1000);
        pItem->setFlag(QGraphicsItem::ItemIsSelectable);
        pItem->setData(0,QVariant::fromValue(set));
        levelsetItems_.append(pItem);

    }

  }


    void SeaScene::resourcesAvailable()
    {
        qDebug() << "Resources available";
        vibrationAllowed_ = true;
    }


    void SeaScene::resourcesLost()
    {
        qDebug() << "Resources lost";
        vibrationAllowed_ = false;
    }
