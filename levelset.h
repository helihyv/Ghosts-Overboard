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

#ifndef LEVELSET_H
#define LEVELSET_H

#include "level.h"
#include <QList>
#include <QString>

class Levelset
{
public:
    Levelset();

    Levelset(QString name, QList<Level> levelList);

    bool isValid();

    QString getName();

    Level getLevel(int index);

    int numberOfLevels();

    int getTotalHighScore();

    void setTotalHighScore(int highScore);

    int getLevelHighScore(int index);

    void setLevelHighScore(int index, int highScore);


private:

    QString name_;

    QList<Level> levels_;

};

#endif // LEVELSET_H
