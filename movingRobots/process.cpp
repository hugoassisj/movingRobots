#include "process.h"

Process::Process()
{
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

void Process::setProcess(int lin, int col)
{
    //process[linha][coluna] =
}
