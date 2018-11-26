#include "vector2d.h"

Vector2D::Vector2D(int x, int y, int sid, int rid)
{
    mutex1 = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_lock( &mutex1 );
    this->x = x;
    this->y = y;
    this->sourceID = sid;
    this->robotID = rid;
    pthread_mutex_unlock( &mutex1 );
}

Vector2D::Vector2D()
{
    mutex1 = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_lock( &mutex1 );
    this->x = -1;
    this->y = -1;
    this->sourceID = -1;
    this->robotID = -1;
    pthread_mutex_unlock( &mutex1 );
}

void Vector2D::set(Vector2D v)
{
    pthread_mutex_lock( &mutex1 );
    this->x = v.x;
    this->y = v.y;
    this->sourceID = v.sourceID;
    this->robotID = v.robotID;
    pthread_mutex_unlock( &mutex1 );
}

Vector2D Vector2D::get()
{
    Vector2D pos;
    pthread_mutex_lock( &mutex1 );
    pos.x = this->x;
    pos.y = this->y;
    pos.sourceID = this->sourceID;
    pos.robotID = this->robotID;
    pthread_mutex_unlock( &mutex1 );
    return pos;
}
