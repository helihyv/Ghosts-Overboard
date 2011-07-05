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
