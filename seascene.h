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


#ifndef SEASCENE_H
#define SEASCENE_H

#include <QGraphicsScene>
#include<QGraphicsItemGroup>
#include "screenlitkeeper.h"
#include "level.h"
#include "levelset.h"
#include <QAction>
#include <QTimer>
#include <QTime>
#include <policy/resource-set.h>

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

    void handleScreenTapped();

    void about();

    void restartLevel();

    void nextLevel();

    void restartGame();

    void forcePause();

    void softContinue();

    void setItemPointersNull();

    void turnPauseOn();

    void resourcesAvailable();

    void resourcesLost();





protected:

    /*! Gives a pointer to a random position if a free one is found. Otherwise returns NULL.
        The position is the upper left corner of a free 40x40 pixels slot.
    */

    QPointF* findRandomFreeSlot();

    void createMenuItems();
    void prepareForMenu(QGraphicsItem * pItem);
    void createAboutBoxItems();
    void createVictoryItems();
    void createLevelCompletedItems();
    void createSelectLevelsetFromListItems();

    const QString ghostImageFilename_;
    const QString rockImageFilename_;
    const QString octopusImageFilename_;
    const QStringList shipImageFilenames_;

    QList<QPointF> freeTiles_;

    int ghostsLeft_;

    QList<QGraphicsItem*> movingItems_;

    bool paused_;

    ScreenLitKeeper screenLitKeeper_;


    int menuItemCount_;

    QGraphicsTextItem * pPausetextItem_;

    QGraphicsTextItem * pRestartLevelItem_;
    QGraphicsTextItem * pRestartGameItem_;
    QGraphicsTextItem * pSettingsItem_;
    QGraphicsTextItem * pAboutItem_;
    QGraphicsTextItem * pQuitItem_;
    QGraphicsTextItem * pChooseLevelsetItem_;

    QGraphicsTextItem * pVictoryScoreItem_;
    QGraphicsTextItem * pAboutBoxItem_;
    QGraphicsTextItem * pVictoryCongratulationsItem_;

    QGraphicsTextItem * pLevelCompletedItem_;

    QGraphicsTextItem * pSelectLevelsetFromListItem_;

    QList<QGraphicsItem *>  levelsetItems_;

    QList<Levelset> availableLevelsets_;

    Levelset levelset_;

    int currentLevel_;


    QAction* pVibrateAction_;

    QAction* pPauseAction_;

    bool pauseForced_;

    QTimer autopauseTimer;


    QTime scoreCounter_;
    int totalScore_;
    int levelScore_;

    ResourcePolicy::ResourceSet * pResourceSet_;
    bool vibrationAllowed_;

};

#endif // SEASCENE_H
