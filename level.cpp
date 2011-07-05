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
