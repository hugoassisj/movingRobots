#ifndef ROBOT_H
#define ROBOT_H

#include <pthread.h>
#include "../Vector2D/vector2d.h"

class Robot
{
    pthread_mutex_t mutexLock;

public:
    Robot(int _id);
    int id;
    int getID();
    Vector2D pos;
    Vector2D getPosition();
    void setPosition(Vector2D newPos);
};

#endif // ROBOT_H
