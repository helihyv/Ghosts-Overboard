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

#ifndef LEVEL_H
#define LEVEL_H

class Level
{
public:
    Level();
    Level(int ghosts, int rocks = 0, int octopuses = 0, int octopusSpeed = 100);
    void setNumberOfGhosts(int ghosts);
    int getNumberOfGhosts() const;
    void setNumberOfRocks(int rocks);
    int getNumberOfRocks() const;
    void setNumberOfOctopuses(int octopuses);
    int getNumberOfOctopuses() const;
    void setOctopusSpeed(int speed);
    int getOctopusSpeed() const;


private:
    int ghosts_;
    int rocks_;
    int octopuses_;
    int octopusSpeed_;

};


#endif // LEVEL_H
