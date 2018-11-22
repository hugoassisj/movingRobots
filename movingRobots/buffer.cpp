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
    bool a;
    pthread_mutex_lock( &mutex1 );
    a = buffer.size();
    pthread_mutex_unlock( &mutex1 );
    if (!a >= 0)
    {
        cout << endl;
        cout << "Size: " << a << endl;
        cout << "--------------------------------------------------" << endl;
        for (int var = 0; var < 25; ++var)
        {
            pthread_mutex_lock( &mutex1 );
            cout << var+1 << ". X: " << buffer[var].x << " Y: " << buffer[var].y << " |SourceID: " << buffer[var].sourceID + 1 << " |RobotID: " << buffer[var].robotID + 1 << endl;
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
//    cout << endl;
//    cout << "Inserido: " <<  endl;
//    for (int robot = 0; robot < 3; ++robot)
//    {
//        cout << "Robot: " << robot + 1 << " |X: " << new_pos[robot].x << " |Y: " << new_pos[robot].y << " |SourceID: " << new_pos[robot].sourceID + 1 << " |RobotID: " << new_pos[robot].robotID + 1 <<endl;
//    }
//    cout << endl;
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
        pthread_mutex_lock( &mutex1 );
        Vector2D element = buffer.front();
        pthread_mutex_unlock( &mutex1 );

        pthread_mutex_lock( &mutex1 );
        buffer.pop_front();
        pthread_mutex_unlock( &mutex1 );
        //cout << endl;
        //cout << "Removido: " << " X: " << element.x << " Y: " << element.y << " |SourceID: " << element.sourceID << " |RobotID: " << element.robotID + 1 << endl;
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
