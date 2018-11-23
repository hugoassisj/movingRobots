#ifndef VECTOR2D_H
#define VECTOR2D_H
#include <pthread.h>


class Vector2D
{
    pthread_mutex_t mutex1;

public:
    int x;
    int y;
    int sourceID;
    int robotID;

    Vector2D();
    Vector2D(int x, int y);
    Vector2D get();
    void set(Vector2D v);
};

#endif // VECTOR2D_H
