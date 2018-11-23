#include "source.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>

using namespace std;

Source::Source(int _id)
{
    mutex1 = PTHREAD_MUTEX_INITIALIZER;
    id = _id;
    time = 0;
}

int Source::getId()
{
    int _id;
    pthread_mutex_lock( &mutex1 );
    _id = id;
    pthread_mutex_unlock( &mutex1 );
    return _id;
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
    int op;
    Vector2D newPos;
    op = (rand() % 2);
    if (op == 0)
    {
        pthread_mutex_lock( &mutex1 );
        newPos.x = r.getPosition().x - (rand() % 20/20 + 1) * 20;
        pthread_mutex_unlock( &mutex1 );
    }
    else
    {
        pthread_mutex_lock( &mutex1 );
        newPos.x = r.getPosition().x + (rand() % 20/20 + 1) * 20;
        pthread_mutex_unlock( &mutex1 );
    }

    op = (rand() % 2);
    if (op == 0)
    {
        pthread_mutex_lock( &mutex1 );
        newPos.y = r.getPosition().y - (rand() % 20/20 + 1) * 20;
        pthread_mutex_unlock( &mutex1 );
    }
    else
    {
        pthread_mutex_lock( &mutex1 );
        newPos.y = r.getPosition().y + (rand() % 20/20 + 1) * 20;
        pthread_mutex_unlock( &mutex1 );
    }

    Saturate(newPos);

    newPos.sourceID = this->getId();
    newPos.robotID = r.getID();
    return newPos;
}
