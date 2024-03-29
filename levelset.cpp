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

#include "levelset.h"
#include <QSettings>
#include <QDebug>

Levelset::Levelset()
{

}

Levelset::Levelset(QString name, QList<Level> levelList)
{
    name_ = name;

    levels_ = levelList;
}


bool Levelset::isValid()
{
    if (name_.isEmpty())
        return false;

    if (levels_.isEmpty())
        return false;

    return true;
}

QString Levelset::getName()
{
    return name_;
}

int Levelset::numberOfLevels()
{
    return levels_.length();
}

Level Levelset::getLevel(int index)
{

    return levels_.value(index); //Returns a default constructed Level if called with invalid index

}

int Levelset::getTotalHighScore()
{
    QSettings settings;
    settings.beginGroup(name_);
    return settings.value("TotalHighScore",900*1000*100).toInt();
}

void Levelset::setTotalHighScore(int highscore)
{
    QSettings settings;

    settings.beginGroup(name_);
    settings.setValue("TotalHighScore", highscore);

}

int Levelset::getLevelHighScore(int index)
{
    QSettings settings;
    QString group = name_;
    group.append("/LevelHighScore");
    settings.beginGroup(group);

//    qDebug() << group;

    return settings.value(QString(index),900*1000).toInt();
}

void Levelset::setLevelHighScore(int index, int highScore)
{
    QSettings settings;
    QString group = name_;
    group.append("/LevelHighScore");
    settings.beginGroup(group);

//    qDebug() << group;

    settings.setValue(QString(index),highScore);
}
