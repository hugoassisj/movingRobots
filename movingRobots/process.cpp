#include "process.h"
#include <iostream>

using namespace std;

Process::Process()
{
    mutex1 = PTHREAD_MUTEX_INITIALIZER;
}

int Process::filterMatrixPosition(int param)
{
    switch (param)
    {
        case 0:
            return 0;
            break;
        case 1:
            return 1;
            break;
        case 2:
            return 2;
            break;
        default:
            return -1;
            break;
    }
}

int Process::getMean(int col)
{

}

void Process::setProcess(int lin, int col, Vector2D pos)
{
    //pthread_mutex_lock( &mutex1 );
    process[lin][col].set(pos);
    //pthread_mutex_unlock( &mutex1 );
}

void Process::display()
{
    cout <<"---------------------------------------------------------------" << endl;
    for (int lin = 0; lin < 3; ++lin)
    {
        for (int col = 0; col < 3; ++col)
        {
            //pthread_mutex_lock( &mutex1 );
            cout << "X: " << process[lin][col].x << "\tY: " << process[lin][col].y << endl;
            //pthread_mutex_unlock( &mutex1 );
        }
        cout << endl;
    }

}

void Process::checkLine()
{

    for (int linha = 0; linha < 3; ++linha)
    {
        pthread_mutex_lock( &mutex1 );
        if (process[linha][0].x != 999 && process[linha][1].x != 999 && process[linha][2].x != 999)
        {

            //getMean(0);
            resetLine(linha);

        }
        pthread_mutex_unlock( &mutex1 );
    }
}

void Process::resetLine(int line)
{
    for (int col = 0; col < 3; ++col)
    {
        //pthread_mutex_lock( &mutex1 );
        process[line][col].x = 999;
        process[line][col].y = 999;
        //pthread_mutex_unlock( &mutex1 );
    }

}
