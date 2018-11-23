#ifndef PROCESS_H
#define PROCESS_H
#include "vector2d.h"
#include <pthread.h>

class Process
{
    Vector2D process[3][3];
    pthread_mutex_t mutex1;

    public:
        Process();
        int filterMatrixPosition(int param);

        void setProcess(int lin, int col, Vector2D pos);
        int getMean(int col);
        void display();
        void checkLine();
        void resetLine(int line);


};

#endif // PROCESS_H
