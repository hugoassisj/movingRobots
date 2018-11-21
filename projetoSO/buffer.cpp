#include "buffer.h"
#include <iostream>
#include <iterator>

using namespace std;

Buffer::Buffer(int max)
{
    maxSize = max;
    mutex1 = PTHREAD_MUTEX_INITIALIZER;
    buffer[10];
}

//Buffer::Buffer()
//{
//}


void Buffer::display()
{
    //mostra o vector
}

void Buffer::putPositions(Vector2D pos)
{
//    cout << buffer.size() << endl;
//    cout << maxSize << endl;
    if (buffer.size() < maxSize)
    {
        pthread_mutex_lock( &mutex1 );
        buffer.push_back(pos);
        pthread_mutex_unlock( &mutex1 );
    }
}

Vector2D * Buffer::getPositions()
{
//    pthread_mutex_lock( &mutex1 );
//    //Aqui tem q desembaralhar geral;
//    Vector2D element = buffer.back();
//    buffer.pop_back();
//    pthread_mutex_unlock( &mutex1 );
//    return element;
}

int Buffer::getSize()
{
    pthread_mutex_lock( &mutex3 );
    return (int)buffer.size();
    pthread_mutex_unlock( &mutex3 );
}

int Buffer::getMaxSize()
{
    pthread_mutex_lock( &mutex2 );
    return maxSize;
    pthread_mutex_unlock( &mutex2 );
}
