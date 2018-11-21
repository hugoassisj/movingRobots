#ifndef SOURCE_H
#define SOURCE_H
#include "robot.h"

class Source
{
    public:
        int id;
        int time;
        Vector2D newPos[3];

        Source(int _id);
        void SetTime(int t);
        int GetTime();
        Vector2D * produce(Robot r[3]);
        void Saturate(Vector2D &pos);
        int getId();
        bool randOperator();
};

#endif // SOURCE_H
