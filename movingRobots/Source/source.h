#ifndef SOURCE_H
#define SOURCE_H

#include <pthread.h>
#include "../Robot/robot.h"
#include "../Vector2D/vector2d.h"

class Source
{
    pthread_mutex_t mutexLock;

public:
    int id;
    int time;

    Source(int _id);
    int getId();
    void SetTime(int newTime);
    int GetTime();
    void Saturate(Vector2D &pos);
    Vector2D produce(Robot robot);
};

#endif // SOURCE_H
