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
    pthread_mutex_lock( &mutex1 );
    if (!buffer.size() >= 0)
    {
        cout << endl;
        cout << "Size: " << buffer.size() << endl;
        cout << "--------------------------------------------------" << endl;
        for (int var = 0; var < maxSize; ++var)
        {
            cout << var+1 << ". X: " << buffer[var].x << " Y: " << buffer[var].y << " |SourceID: " << buffer[var].sourceID + 1 << " |RobotID: " << buffer[var].robotID + 1 << endl;

        }
    }
    pthread_mutex_unlock( &mutex1 );
}

void Buffer::init()
{
    pthread_mutex_lock( &mutex1 );
    for (int var = 0; var < maxSize; ++var)
    {
        buffer[var].x = -2;
        buffer[var].y = -2;
        buffer[var].sourceID = -2;
        //buffer[var].robotID = -2;
    }
    pthread_mutex_unlock( &mutex1 );
}

void Buffer::putPositions(Vector2D pos)
{
    cout << endl;
    cout << "Inserido: " << " X: " << pos.x << " Y: " << pos.y << " |SourceID: "  << pos.sourceID << " |RobotID: " << pos.robotID + 1 <<  endl;
    if (buffer.size() < maxSize)
    {
        pthread_mutex_lock( &mutex1 );
        buffer.push_back(pos);
        pthread_mutex_unlock( &mutex1 );
    }
}

Vector2D Buffer::getPositions()
{
    if (buffer.size() > 0)
    {
        pthread_mutex_lock( &mutex1 );
        Vector2D element = buffer.front();
        buffer.pop_front();
        pthread_mutex_unlock( &mutex1 );
        cout << endl;
        cout << "Removido: " << " X: " << element.x << " Y: " << element.y << " |SourceID: " << element.sourceID << " |RobotID: " << element.robotID + 1 << endl;
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
