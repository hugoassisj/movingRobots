#include "source.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>

#define NEW_POS_RANGE 6

using namespace std;

Source::Source(int _id)
{
    mutex1 = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_lock( &mutex1 );
    id = _id;
    time = 0;
    pthread_mutex_unlock( &mutex1 );
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
    if (pos.x > 370)
    {
        pos.x = pos.x - 370;
    }
    else if (pos.x < -10)
    {
        pos.x = 370 + pos.x;
    }

    if (pos.y > 270)
    {
        pos.y = pos.y - 270;
    }
    else if (pos.y < -10)
    {
        pos.y = 270 + pos.y;
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
        newPos.x = r.getPosition().x - (rand() % NEW_POS_RANGE);
        pthread_mutex_unlock( &mutex1 );
    }
    else
    {
        pthread_mutex_lock( &mutex1 );
        newPos.x = r.getPosition().x + (rand() % NEW_POS_RANGE);
        pthread_mutex_unlock( &mutex1 );
    }

    op = (rand() % 2);
    if (op == 0)
    {
        pthread_mutex_lock( &mutex1 );
        newPos.y = r.getPosition().y - (rand() % NEW_POS_RANGE);
        pthread_mutex_unlock( &mutex1 );
    }
    else
    {
        pthread_mutex_lock( &mutex1 );
        newPos.y = r.getPosition().y + (rand() % NEW_POS_RANGE);
        pthread_mutex_unlock( &mutex1 );
    }

    pthread_mutex_lock( &mutex1 );
    Saturate(newPos);
    pthread_mutex_unlock( &mutex1 );

    int sid_ = this->getId();
    int rid_ = r.getID();

    //pthread_mutex_lock( &mutex1 );
    newPos.sourceID = sid_;
    newPos.robotID = rid_;
    //pthread_mutex_unlock( &mutex1 );

    return newPos;
}
