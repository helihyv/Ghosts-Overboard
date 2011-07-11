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


#include "level.h"

Level::Level()
{
    setNumberOfGhosts(5);
    setNumberOfRocks(0);
    setNumberOfOctopuses(0);
    setOctopusSpeed(100);
}

Level::Level(int ghosts, int rocks, int octopuses, int octopusSpeed)
{
    setNumberOfGhosts(ghosts);
    setNumberOfRocks(rocks);
    setNumberOfOctopuses(octopuses);
    setOctopusSpeed(octopusSpeed);
}

int  Level::getNumberOfGhosts() const
{
    return ghosts_;
}

void Level::setNumberOfGhosts(int ghosts)
{
    ghosts_ = ghosts;
}

int Level::getNumberOfRocks() const
{
    return rocks_;
}

void Level::setNumberOfRocks(int rocks)
{
    rocks_ = rocks;
}

int Level::getNumberOfOctopuses() const
{
    return octopuses_;
}

void Level::setNumberOfOctopuses(int octopuses)
{
    octopuses_ = octopuses;
}

int Level::getOctopusSpeed() const
{
    return octopusSpeed_;
}

void Level::setOctopusSpeed(int speed)
{
    octopusSpeed_ = speed;
}
