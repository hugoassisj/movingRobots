#include "robot.h"
#include <stdlib.h>
#include <time.h>


Robot::Robot(int id)
{
    mutex1 = PTHREAD_MUTEX_INITIALIZER;
    static bool seeded = false;
    if(!seeded)
    {
        srand(time(NULL));
        seeded = true;
    }

   pos.x = (rand() % 18)*20;
   pos.y = (rand() % 14)*20;
   this->id = id;
}

void Robot::setPosition(Vector2D p)
{
    pthread_mutex_lock( &mutex1 );
    pos.x = p.x;
    pos.y = p.y;
    pthread_mutex_unlock( &mutex1 );
}

Vector2D Robot::getPosition()
{
    Vector2D a;
    pthread_mutex_lock( &mutex1 );
    a.x = pos.x;
    a.y = pos.y;
    pthread_mutex_unlock( &mutex1 );
    return a;
}

int Robot::getID()
{
    int a;
    pthread_mutex_lock( &mutex1 );
    a = id;
    pthread_mutex_unlock( &mutex1 );
    return a;
}

