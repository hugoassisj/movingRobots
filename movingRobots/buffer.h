#ifndef BUFFER_H
#define BUFFER_H
#include "source.h"
#include <vector>
#include <pthread.h>
#include <deque>


class Buffer
{
    pthread_mutex_t mutex1;
    //std::vector<Vector2D> buffer;
    int maxSize;
    public:
    std::deque<Vector2D> buffer;
        Buffer(int max);
        void putPositions(Vector2D pos);
        Vector2D getPositions();
        void display();
        int getSize();
        int getMaxSize();
        void init();
        Vector2D consult();
};

#endif // BUFFER_H
