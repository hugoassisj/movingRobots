#ifndef ROBOT_H
#define ROBOT_H
#include "vector2d.h"

class Robot
{
    public:
        Robot();
        Vector2D pos;
        Vector2D getPosition();
        void setPosition(Vector2D p);
};

#endif // ROBOT_H
