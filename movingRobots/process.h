#ifndef PROCESS_H
#define PROCESS_H
#include "vector2d.h"
#include <pthread.h>

class Process
{

    pthread_mutex_t mutex1;

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

#endif // PROCESS_H
