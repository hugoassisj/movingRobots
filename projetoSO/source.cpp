#include "source.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>

using namespace std;

Source::Source(int _id)
{
    id = _id;
    time = 0;
}

int Source::getId()
{
    return id;
}

void Source::SetTime(int t)
{
    time = t;
}

int Source::GetTime()
{
    return time;
}

//Quando estoura a tela, volta ao inicio pelo outro lado
void Source::Saturate(Vector2D &pos)
{
    if (pos.x > 380)
    {
        pos.x = pos.x - 380;
    }
    else if (pos.x < 0)
    {
        pos.x = 400 - pos.x;
    }

    if (pos.y > 280)
    {
        pos.y = pos.y - 280;
    }
    else if (pos.y < 0)
    {
        pos.y = 300 - pos.y;
    }
}

//Produz Posições para os 3 robôs
Vector2D * Source::produce(Robot r[3])
{
    int op;
    for (int robot = 0; robot < 3; ++robot)
    {
        op = (rand() % 2);
        if (op == 0)
        {
            newPos[robot].x = r[robot].getPosition().x - (rand() % 40/20 +1)*20;
        }
        else
        {
            newPos[robot].x = r[robot].getPosition().x - (rand() % 40/20 +1)*20;
        }

        op = (rand() % 2);
        if (op == 0)
        {
            newPos[robot].y = r[robot].getPosition().y + (rand() % 40/20 +1)*20;
        }
        else
        {
            newPos[robot].y = r[robot].getPosition().y + (rand() % 40/20 +1)*20;
        }
        Saturate(newPos[robot]);
    }
    return newPos;
}

bool Source::randOperator()
{
    int op = (rand() % 2);

    if (op == 0)
    {

    }
}
