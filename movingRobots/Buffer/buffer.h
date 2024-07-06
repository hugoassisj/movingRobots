#ifndef BUFFER_H
#define BUFFER_H

#include <pthread.h>
#include <deque>
#include "../Vector2D/vector2d.h"

class Buffer
{
    pthread_mutex_t mutexLock;
    int maxSize;

public:
    std::deque<Vector2D> buffer;
    Buffer(int _maxSize);
    void putPositions(Vector2D pos);
    Vector2D getPositions();
    void display();
    int getSize();
    int getMaxSize();
    void init();
    Vector2D consult();
};

#endif // BUFFER_H
