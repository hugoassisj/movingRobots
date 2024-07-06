#include <iostream>
#include "processor.h"

using namespace std;

/**
 * @brief Constructs a Process object.
 * 
 * Initializes the mutex lock for thread safety.
 */
Process::Process()
{
    mutexLock = PTHREAD_MUTEX_INITIALIZER;
}

/**
 * @brief Maps a parameter to a matrix position.
 * 
 * @param param The parameter value.
 * @return int The corresponding matrix position.
 */
int Process::filterMatrixPosition(int param)
{
    switch (param)
    {
    case 0:
        return 0;
    case 1:
        return 1;
    case 2:
        return 2;
    default:
        return -1;
    }
}

/**
 * @brief Calculates the mean position for a given line in the process matrix.
 * 
 * @param line The line index.
 * @return Vector2D The mean position.
 */
Vector2D Process::getMean(int line)
{
    pthread_mutex_lock(&mutexLock);
    int meanX = (process[line][0].x + process[line][1].x + process[line][2].x) / 3;
    int meanY = (process[line][0].y + process[line][1].y + process[line][2].y) / 3;
    pthread_mutex_unlock(&mutexLock);

    return Vector2D(meanX, meanY, 999, line);
}

/**
 * @brief Sets a position in the process matrix.
 * 
 * @param row The row index.
 * @param col The column index.
 * @param pos The position vector to set.
 */
void Process::setProcess(int row, int col, Vector2D pos)
{
    pthread_mutex_lock(&mutexLock);
    process[row][col].set(pos);
    pthread_mutex_unlock(&mutexLock);
}

/**
 * @brief Displays the process matrix.
 */
void Process::display()
{
    cout << "---------------------------------------------------------------" << endl;
    for (int row = 0; row < 3; ++row)
    {
        for (int col = 0; col < 3; ++col)
        {
            pthread_mutex_lock(&mutexLock);
            cout << "X: " << process[row][col].x << "\tY: " << process[row][col].y << "\t|\t";
            pthread_mutex_unlock(&mutexLock);
        }
        cout << endl;
    }
}

/**
 * @brief Checks each line of the process matrix for complete positions.
 * 
 * If a line is complete, calculates the mean position and resets the line.
 * 
 * @return Vector2D The mean position if found, otherwise a default position.
 */
Vector2D Process::checkLine()
{
    for (int line = 0; line < 3; ++line)
    {
        pthread_mutex_lock(&mutexLock);
        int x1 = process[line][0].x;
        int x2 = process[line][1].x;
        int x3 = process[line][2].x;
        pthread_mutex_unlock(&mutexLock);

        if (x1 != 999 && x2 != 999 && x3 != 999)
        {
            Vector2D nPos = getMean(line);
            resetLine(line);
            return nPos;
        }
    }
    return Vector2D(999, 999, 999, 999);
}

/**
 * @brief Resets a line in the process matrix to default values.
 * 
 * @param line The line index to reset.
 */
void Process::resetLine(int line)
{
    for (int col = 0; col < 3; ++col)
    {
        pthread_mutex_lock(&mutexLock);
        process[line][col].x = 999;
        process[line][col].y = 999;
        pthread_mutex_unlock(&mutexLock);
    }
}

/**
 * @brief Checks if there are available positions in the process matrix.
 * 
 * @return true If there are available positions.
 * @return false If there are no available positions.
 */
bool Process::available()
{
    int cont = 0;
    for (int row = 0; row < 3; ++row)
    {
        for (int col = 0; col < 3; ++col)
        {
            pthread_mutex_lock(&mutexLock);
            int x = process[row][col].x;
            int y = process[row][col].y;
            pthread_mutex_unlock(&mutexLock);

            if (x == 999 && y == 999)
            {
                cont += 1;
            }
        }
    }
    return cont > 0;
}