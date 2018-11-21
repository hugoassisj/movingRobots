#include "mainwindow.h"
#include <QApplication>
#include <pthread.h>
#include <QThread>
#include <iostream>
#include <unistd.h>
#include "robot.h"
#include "buffer.h"
#include "source.h"

#define BUFFER_SIZE 10

using namespace std;


Robot r1 = Robot();
Robot r2 = Robot();
Robot r3 = Robot();

mainWindow *UIptr;

Robot allbots[3] = {r1, r2, r3};

struct sources
{
    Source s1 = Source(0);
    Source s2 = Source(1);
    Source s3 = Source(2);
};

Buffer b(BUFFER_SIZE);

sources ss;

Vector2D * new_pos;

//Source_Thread1------------------------------------------------------------------------------------
void *Source_Thread1(void* ptr)
{
    cout << (char *) ptr << endl;
    while (true)
    {

        //Source Produz uma nova Posição pra cada Robô
        new_pos = ss.s1.produce(allbots);

        //Mostra essas Novas Posições
        cout <<"Source: " << ss.s1.getId() + 1 << "  -------------------------------------------------------------------"<< endl;
        for (int robot = 0; robot < 3; ++robot)
        {
            cout << "Robot: " << robot + 1 << " |X: " << new_pos[robot].x << " |Y: " << new_pos[robot].y << endl;
        }
        cout << endl;

        //Posição é Passada ao Buffer
        b.putPositions(*new_pos);

        usleep(UIptr->getSlider1Value()*1000);
    }
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------



//Source_Thread2------------------------------------------------------------------------------------
void *Source_Thread2(void* ptr)
{
    cout << (char *) ptr << endl;
    while (true)
    {
        //Source Produz uma nova Posição pra cada Robô
        new_pos = ss.s2.produce(allbots);

        //Mostra essas Novas Posições
        cout <<"Source: " << ss.s2.getId() + 1 << "  -------------------------------------------------------------------"<< endl;
        for (int robot = 0; robot < 3; ++robot)
        {
            cout << "Robot: " << robot + 1 << " |X: " << new_pos[robot].x << " |Y: " << new_pos[robot].y << endl;
        }
        cout << endl;

        //Posição é Passada ao Buffer
        //b.populate(s2, npos);

        usleep(UIptr->getSlider2Value()*1000);
    }
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------



//Source_Thread3------------------------------------------------------------------------------------
void *Source_Thread3(void* ptr)
{
    cout << (char *) ptr << endl;
    while (true)
    {
        //Source Produz uma nova Posição pra cada Robô
        new_pos = ss.s3.produce(allbots);

        //Mostra essas Novas Posições
        cout <<"Source: " << ss.s3.getId() + 1 << "  -------------------------------------------------------------------"<< endl;
        for (int robot = 0; robot < 3; ++robot)
        {
            cout << "Robot: " << robot + 1 << " |X: " << new_pos[robot].x << " |Y: " << new_pos[robot].y << endl;
        }
        cout << endl;

        //Posição é Passada ao Buffer
        //b.populate(s1, npos);

       usleep(UIptr->getSlider3Value()*1000);
    }
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------



//Buffer_Thread-------------------------------------------------------------------------------------
void *Buffer_Thread(void* ptr)
{
    while (true)
    {
        usleep(100000);
    }
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------

int qtd = 0;

//Process_Thread------------------------------------------------------------------------------------
void *Process_Thread(void* ptr)
{
    cout << (char *) ptr << endl;
    while (true)
    {
        //UIptr->updateProgressBar(3, 10);
        //Subscreve posição do Buffer - Vector2D
        //Soma, as posiçoes de cada fonte e tira a média
        //Publica para Thread de Posicionamento
        //UIptr->DefineBuffer(b.getSize(), b.getMaxSize());
        if(qtd == 10) qtd = 0;
//        UIptr->updateBufferMax(BUFFER_SIZE);
//        proce;

        usleep(1000000);
    }
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    mainWindow w;
    UIptr = &w;

    w.DefineSources(ss.s1,ss.s2,ss.s3);
    w.DefineBuffer(&b);
    w.DefineRobos(r1, r2, r3);
    w.PosicionaRobos(r1,r2,r3);
    w.ShowPositions(r1,r2,r3);
    w.show();

    //UIptr->updateProgressBar(b.getSize(), b.getMaxSize());

    //--------------------------------------------------------------------------------------------------
    //Cria Threads Buffer
    //--------------------------------------------------------------------------------------------------
    pthread_t BufferThread;
    const char *BufferMessage = "Criacao do Buffer Iniciada";
    int BufferRet = pthread_create( &BufferThread, NULL, Buffer_Thread, (void*) BufferMessage);
    if(BufferRet)
    {
        cout << "Failed to Create Buffer Thread" << endl;
    }
    //--------------------------------------------------------------------------------------------------
    //--------------------------------------------------------------------------------------------------




    //--------------------------------------------------------------------------------------------------
    //Cria Threads Processador
    //--------------------------------------------------------------------------------------------------
    pthread_t ProcessThread;
    const char *ProcessMessage = "Criacao do Buffer Iniciada";
    int ProcessRet = pthread_create( &ProcessThread, NULL, Process_Thread, (void*) ProcessMessage);
    if(ProcessRet)
    {
        cout << "Failed to Create Process Thread" << endl;
    }
    //--------------------------------------------------------------------------------------------------
    //--------------------------------------------------------------------------------------------------




    //--------------------------------------------------------------------------------------------------
    //Cria Threads Source
    //--------------------------------------------------------------------------------------------------
    pthread_t SourceThread1, SourceThread2, SourceThread3;

    const char *SourceMessage1 = "Criacao de Source 1 Iniciada";
    const char *SourceMessage2 = "Criacao de Source 2 Iniciada";
    const char *SourceMessage3 = "Criacao de Source 3 Iniciada";

    int SourceRet1;
    SourceRet1 = pthread_create( &SourceThread1, NULL, Source_Thread1, (void*) &w);
    if(SourceRet1)
    {
        cout << "Failed to Create Source Thread 1" << endl;
    }
    usleep(5000);

    int SourceRet2;
    SourceRet2 = pthread_create( &SourceThread2, NULL, Source_Thread2, (void*) SourceMessage2);
    if(SourceRet2)
    {
        cout << "Failed to Create Source Thread 2" << endl;
    }

    usleep(5000);
    int SourceRet3;
    SourceRet3 = pthread_create( &SourceThread3, NULL, Source_Thread3, (void*) SourceMessage3);
    if(SourceRet3)
    {
        cout << "Failed to Create Source Thread 3" << endl;
    }
    //--------------------------------------------------------------------------------------------------
    //--------------------------------------------------------------------------------------------------











    return a.exec();
}

