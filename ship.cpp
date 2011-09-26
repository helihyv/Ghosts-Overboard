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


#include "ship.h"
#include <QDebug>
#include <QDBusMessage>
#include <QDBusConnection>
#include <QFeedbackHapticsEffect>



QTM_USE_NAMESPACE


Ship::Ship(QList<QPixmap> pixmapList, QGraphicsItem *parent) :
    OrientationControlledGraphicsPixmapObject(pixmapList.at(0),parent)
{
    shipImages_ = pixmapList;
    ghostsAboard_ = 0;
    vibrationActive_ = false;
    vibrationAllowed_ = false;
}

bool Ship::handleCollisions()
{
    QList<QGraphicsItem*>  collidesList = collidingItems();

    if (collidesList.isEmpty())
        return true;

    else
    {
        //since the game logic does not leave items to collide with each other we can take just the topmost one
        //and trust it is the only one
        QString type = collidesList.at(0)->data(0).toString();
//        qDebug() << type;

        if (type == "rock" || type == "octopus")
        {
            // drop all ghosts when hitting an obstacle

            dropAllGhosts();

            //go back to old position

            return false;
        }

        else if (type == "ghost")
        {
            ghostsAboard_++;
            updateShipImage();

//            qDebug() << ghostsAboard_ << " ghosts aboard";

            emit pickingGhost(collidesList.at(0));

            return true;
        }

    }


    return true; //execution can never reach here, this is just to stop the compiler from complaining
}

void Ship::updateShipImage()
{
    int index = qBound(0,ghostsAboard_,shipImages_.length()-1);
    setPixmap(shipImages_.at(index));
}

void Ship::dropAllGhosts()
{

    emit droppingGhosts(ghostsAboard_);
    ghostsAboard_ = 0;
    updateShipImage();

    //vibrate

    if (vibrationActive_ && vibrationAllowed_)
    {

 //       This is for fremantle
//        QDBusMessage message = QDBusMessage::createMethodCall("com.nokia.mce","/com/nokia/mce/request","com.nokia.mce.request","req_vibrator_pattern_activate");

//        QList<QVariant> arguments;

//        arguments.append("PatternChatAndEmail");

//        message.setArguments(arguments);

//        message = QDBusConnection::systemBus().call(message);

    //qDebug() << message;


        //This is for Harmattan

        QFeedbackHapticsEffect vibrationEffect;
        vibrationEffect.setDuration(1000);
        vibrationEffect.setIntensity(1.0);
        vibrationEffect.start();

    }
}

void Ship::setVibrationActivate(bool on)
{
    vibrationActive_ = on;
}

void Ship::allowVibration()
{
    vibrationAllowed_ = true;
}

void Ship::disallowVibration()
{
    vibrationAllowed_ = false;
}
