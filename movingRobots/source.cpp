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
        pos.x = pos.x - 380 - 20;
    }
    else if (pos.x < 0)
    {
        pos.x = 380 - pos.x - 20;
    }

    if (pos.y > 280)
    {
        pos.y = pos.y - 280 - 20;
    }
    else if (pos.y < 0)
    {
        pos.y = 280 - pos.y - 20;
    }
}

//Produz Posições para os 3 robôs
Vector2D Source::produce(Robot r)
{
    op = (rand() % 2);
    if (op == 0)
    {
        newPos.x = r.getPosition().x - (rand() % 20/20 + 1) * 20;
    }
    else
    {
        newPos.x = r.getPosition().x + (rand() % 20/20 + 1) * 20;
    }

    op = (rand() % 2);
    if (op == 0)
    {
        newPos.y = r.getPosition().y - (rand() % 20/20 + 1) * 20;
    }
    else
    {
        newPos.y = r.getPosition().y + (rand() % 20/20 + 1) * 20;
    }
    Saturate(newPos);
    return newPos;
}
