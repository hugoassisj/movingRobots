#ifndef BUFFER_H
#define BUFFER_H
#include "source.h"
#include <vector>
#include <pthread.h>


class Buffer
{
    pthread_mutex_t mutex1;
    std::vector<Vector2D> buffer;
    int maxSize;
    public:
        Buffer(int max);
        void putPositions(Vector2D pos);
        Vector2D * getPositions();
        void display();
        int getSize();
        int getMaxSize();
};

#endif // BUFFER_H
