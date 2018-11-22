#ifndef PROCESS_H
#define PROCESS_H
#include "vector2d.h"

class Process
{
    Vector2D process[3][3];

    public:
        Process();
        int filterMatrixPosition(int param);

        void setProcess(int lin, int col);
        int getMean(int col);


};

#endif // PROCESS_H
