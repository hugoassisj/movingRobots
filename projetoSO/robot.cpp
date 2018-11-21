#include "robot.h"
#include <stdlib.h>
#include <time.h>


Robot::Robot()
{
    static bool seeded = false;
    if(!seeded)
    {
        srand(time(NULL));
        seeded = true;
    }

   pos.x = (rand() % 18)*20;
   pos.y = (rand() % 14)*20;
}

void Robot::setPosition(Vector2D p)
{
    pos.x = p.x;
    pos.y = p.y;
}

Vector2D Robot::getPosition()
{
    return pos;
}
