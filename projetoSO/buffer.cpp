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
