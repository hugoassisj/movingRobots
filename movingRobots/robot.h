#ifndef ROBOT_H
#define ROBOT_H
#include "vector2d.h"
#include <pthread.h>

class Robot
{

    pthread_mutex_t mutex1;

public:
    int id;
    Robot(int id);
    Vector2D pos;
    Vector2D getPosition();
    void setPosition(Vector2D p);
    int getID();
};

#endif // ROBOT_H
