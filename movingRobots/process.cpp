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

Vector2D Process::getMean(int linha)
{
    pthread_mutex_lock( &mutex1 );
    int meanX = (process[linha][0].x + process[linha][1].x + process[linha][2].x)/3;
    int meanY = (process[linha][0].y + process[linha][1].y + process[linha][2].y)/3;
    pthread_mutex_unlock( &mutex1 );

    //cout << endl << "MEAN: X: " << meanX << "\tY: " << meanY << endl;
    return Vector2D(meanX, meanY, 999, linha);
}

void Process::setProcess(int lin, int col, Vector2D pos)
{
    pthread_mutex_lock( &mutex1 );
    process[lin][col].set(pos);
    pthread_mutex_unlock( &mutex1 );
}

void Process::display()
{
    cout <<"---------------------------------------------------------------" << endl;
    for (int lin = 0; lin < 3; ++lin)
    {
        for (int col = 0; col < 3; ++col)
        {
            pthread_mutex_lock( &mutex1 );
            cout << "X: " << process[lin][col].x << "\tY: " << process[lin][col].y << "\t|\t";
            pthread_mutex_unlock( &mutex1 );
        }
        cout << endl;
    }

}

Vector2D Process::checkLine()
{

    for (int linha = 0; linha < 3; ++linha)
    {
        pthread_mutex_lock( &mutex1 );
        int x1 = process[linha][0].x;
        int x2 = process[linha][1].x;
        int x3 = process[linha][2].x;
        pthread_mutex_unlock( &mutex1 );
        if (x1 != 999 && x2 != 999 && x3 != 999)
        {
            Vector2D nPos = getMean(linha);
            resetLine(linha);
            return nPos;
        }
    }
    return Vector2D(999,999,999,999);
}

void Process::resetLine(int line)
{
    for (int col = 0; col < 3; ++col)
    {
        pthread_mutex_lock( &mutex1 );
        process[line][col].x = 999;
        process[line][col].y = 999;
        pthread_mutex_unlock( &mutex1 );
    }

}

bool Process::available()
{
    int cont = 0;
    for (int l = 0; l < 3; ++l)
    {
        for (int c = 0; c < 3; ++c)
        {
            pthread_mutex_lock( &mutex1 );
            int x = process[l][c].x;
            int y = process[l][c].y;
            pthread_mutex_unlock( &mutex1 );
            if (x == 999 && y == 999)
            {
                cont += 1;
            }
        }
    }
    if (cont > 0)
    {
       return true;
    }
    else
    {
        return false;
    }
}
