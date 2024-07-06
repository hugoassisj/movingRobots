#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <pthread.h>
#include "../Vector2D/vector2d.h"

class Process
{
    pthread_mutex_t mutexLock;

    public:
        Vector2D process[3][3];

        Process();
        int filterMatrixPosition(int param);
        void setProcess(int lin, int col, Vector2D pos);
        Vector2D getMean(int col);
        void display();
        Vector2D checkLine();
        void resetLine(int line);
        bool available();
};

#endif // PROCESSOR_H
