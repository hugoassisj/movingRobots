#include "buffer.h"
#include <iostream>
#include <iterator>

using namespace std;

Buffer::Buffer(int max)
{
    maxSize = max;
    mutex1 = PTHREAD_MUTEX_INITIALIZER;
    buffer[maxSize];
    //init(); //PQ DA PAU AQUI CARAIO
}

void Buffer::display()
{
    int a;
    pthread_mutex_lock( &mutex1 );
    a = buffer.size();
    pthread_mutex_unlock( &mutex1 );
    if (a >= 0)
    {
        cout << endl;
        cout << "Size: " << a << endl;
        cout << "--------------------------------------------------" << endl;
        for (int var = 0; var < a; ++var)
        {
            pthread_mutex_lock( &mutex1 );
            cout << var+1 << ".\tX: " << buffer[var].x << "\tY: " << buffer[var].y << "\t|SourceID: " << buffer[var].sourceID << "\t|RobotID: " << buffer[var].robotID << endl;
            pthread_mutex_unlock( &mutex1 );
        }
    }

}

void Buffer::init()
{

    for (int var = 0; var < maxSize; ++var)
    {
        pthread_mutex_lock( &mutex1 );
        buffer[var].x = -2;
        buffer[var].y = -2;
        buffer[var].sourceID = -2;
        buffer[var].robotID = -2;
        pthread_mutex_unlock( &mutex1 );
    }

}

void Buffer::putPositions(Vector2D pos)
{
    int bs;
    pthread_mutex_lock( &mutex1 );
    bs = buffer.size();
    pthread_mutex_unlock( &mutex1 );
    if (bs < maxSize)
    {
        pthread_mutex_lock( &mutex1 );
        buffer.push_back(pos);
        pthread_mutex_unlock( &mutex1 );
    }
}

Vector2D Buffer::getPositions()
{
    int bs;
    pthread_mutex_lock( &mutex1 );
    bs = buffer.size();
    pthread_mutex_unlock( &mutex1 );
    if (bs > 0)
    {
        Vector2D element;
        pthread_mutex_lock( &mutex1 );
        element = buffer.front();
        pthread_mutex_unlock( &mutex1 );

        pthread_mutex_lock( &mutex1 );
        buffer.pop_front();
        pthread_mutex_unlock( &mutex1 );
        return element;
    }
}

Vector2D Buffer::consult()
{
    int bs;
    pthread_mutex_lock( &mutex1 );
    bs = buffer.size();
    pthread_mutex_unlock( &mutex1 );
    if (bs > 0)
    {
        Vector2D element;
        pthread_mutex_lock( &mutex1 );
        element = buffer.front();
        pthread_mutex_unlock( &mutex1 );
        return element;
    }
}

int Buffer::getSize()
{
    int s;
    pthread_mutex_lock( &mutex1 );
    s = (int) buffer.size();
    pthread_mutex_unlock( &mutex1 );
    return s;
}

int Buffer::getMaxSize()
{
    int s;
    pthread_mutex_lock( &mutex1 );
    s = maxSize;
    pthread_mutex_unlock( &mutex1 );
    return s;
}
