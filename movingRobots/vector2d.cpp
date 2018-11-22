#include "vector2d.h"

Vector2D::Vector2D(int x, int y)
{
    this->x = x;
    this->y = y;
}

Vector2D::Vector2D()
{
    this->x = 999;
    this->y = 999;
    this->sourceID = 999;
    this->robotID = 999;
}
