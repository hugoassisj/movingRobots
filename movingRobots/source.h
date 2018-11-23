#ifndef SOURCE_H
#define SOURCE_H
#include "robot.h"
#include <pthread.h>

class Source
{
    pthread_mutex_t mutex1;

public:
    int id;
    int time;

    Source(int _id);
    void SetTime(int t);
    int GetTime();
    Vector2D produce(Robot r);
    void Saturate(Vector2D &pos);
    int getId();
};

#endif // SOURCE_H
