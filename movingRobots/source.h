#ifndef SOURCE_H
#define SOURCE_H
#include "robot.h"

class Source
{
    public:
        int id;
        int time;
        int op;
        Vector2D newPos;

        Source(int _id);
        void SetTime(int t);
        int GetTime();
        Vector2D produce(Robot r);
        void Saturate(Vector2D &pos);
        int getId();
};

#endif // SOURCE_H
