#include "mainwindow.h"
#include <QApplication>
#include <pthread.h>
#include <QThread>
#include <iostream>
#include <unistd.h>
#include "robot.h"
#include "buffer.h"
#include "source.h"
#include "process.h"

#define BUFFER_SIZE 30

using namespace std;


Robot r1 = Robot(0);
Robot r2 = Robot(1);
Robot r3 = Robot(2);

Source s1 = Source(0);
Source s2 = Source(1);
Source s3 = Source(2);

mainWindow *UIptr;

Buffer b1(BUFFER_SIZE);

Process p1;

int robotID;
int sourceID;

Vector2D  new_pos[3];

//Source_Thread1------------------------------------------------------------------------------------
void *Source_Thread1(void* ptr)
{
    cout << (char *) ptr << endl;
    while (true)
    {

        //Source Produz uma nova Posição pra cada Robô
        new_pos[0] = s1.produce(r1);
        new_pos[1] = s1.produce(r2);
        new_pos[2] = s1.produce(r3);

        //Mostra essas Novas Posições
        cout <<"Source: " << s1.getId() + 1 << "  -------------------------------------------------------------------"<< endl;
        for (int robot = 0; robot < 3; ++robot)
        {
            cout << "Robot: " << robot + 1 << " |X: " << new_pos[robot].x << " |Y: " << new_pos[robot].y << " |SourceID: " << new_pos[robot].sourceID + 1 << " |RobotID: " << new_pos[robot].robotID + 1 <<endl;
        }
        cout << endl;

        //Posição é Passada ao Buffer
        //b1.putPositions(*new_pos);

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
        new_pos[0] = s2.produce(r1);
        new_pos[1] = s2.produce(r2);
        new_pos[2] = s2.produce(r3);

         //Mostra essas Novas Posições
        for (int robot = 0; robot < 3; ++robot)
        {
            cout << "Robot: " << robot + 1 << " |X: " << new_pos[robot].x << " |Y: " << new_pos[robot].y << " |SourceID: " << new_pos[robot].sourceID + 1 << " |RobotID: " << new_pos[robot].robotID + 1 <<endl;
        }
        cout << endl;

        //Posição é Passada ao Buffer
        b1.putPositions(*new_pos);

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
        new_pos[0] = s3.produce(r1);
        new_pos[1] = s3.produce(r2);
        new_pos[2] = s3.produce(r3);

        //Mostra essas Novas Posições
        cout <<"Source: " << s3.getId() + 1 << "  -------------------------------------------------------------------"<< endl;
        for (int robot = 0; robot < 3; ++robot)
        {int robotId;
            cout << "Robot: " << robot + 1 << " |X: " << new_pos[robot].x << " |Y: " << new_pos[robot].y << " |SourceID: " << new_pos[robot].sourceID + 1 << " |RobotID: " << new_pos[robot].robotID + 1 <<endl;
        }
        cout << endl;

        //Posição é Passada ao Buffer
        b1.putPositions(*new_pos);

       usleep(UIptr->getSlider3Value()*1000);
    }
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------



//Buffer_Thread-------------------------------------------------------------------------------------
void *Buffer_Thread(void* ptr)
{
    cout << (char *) ptr << endl;
    while (true)
    {
        usleep(100000);
    }
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------




//Process_Thread------------------------------------------------------------------------------------
void *Process_Thread(void* ptr)
{
    cout << (char *) ptr << endl;
    while (true)
    {
       // robotID = b1.getPositions().robotID;
       // sourceID = b1.getPositions().sourceID;

       // p1.setProcess(p1.filterMatrixPosition(robotID), p1.filterMatrixPosition(sourceID), );


//        cout << b1.getPositions().x << " " << b1.getPositions().y << endl;
//        b1.display();
        //UIptr->updateProgressBar(3, 10);
        //Subscreve posição do Buffer - Vector2D
        //Soma, as posiçoes de cada fonte e tira a média
        //Publica para Thread de Posicionamento
        usleep(2000000);
    }
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------






//--------------------------------------------------------------------------------------------------
// MAIN
//--------------------------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    mainWindow w;
    UIptr = &w;

    w.DefineSources(&s1, &s2, &s3);
    w.DefineBuffer(&b1);
    w.DefineRobos(&r1, &r2, &r3);

    w.show();

    //---------------------------------------------------------------------------------------------
    //Cria Threads Buffer
    //---------------------------------------------------------------------------------------------
    pthread_t BufferThread;
    const char *BufferMessage = "Criacao do Buffer Iniciada";
    int BufferRet = pthread_create( &BufferThread, NULL, Buffer_Thread, (void*) BufferMessage);
    if(BufferRet)
    {
        cout << "Failed to Create Buffer Thread" << endl;
    }
    usleep(5000);
    //---------------------------------------------------------------------------------------------
    //---------------------------------------------------------------------------------------------




    //---------------------------------------------------------------------------------------------
    //Cria Threads Processador
    //---------------------------------------------------------------------------------------------
    pthread_t ProcessThread;
    const char *ProcessMessage = "Criacao do Process Iniciada";
    int ProcessRet = pthread_create( &ProcessThread, NULL, Process_Thread, (void*) ProcessMessage);
    if(ProcessRet)
    {
        cout << "Failed to Create Process Thread" << endl;
    }
    usleep(5000);
    //---------------------------------------------------------------------------------------------
    //---------------------------------------------------------------------------------------------




    //---------------------------------------------------------------------------------------------
    //Cria Threads Source
    //---------------------------------------------------------------------------------------------
    pthread_t SourceThread1, SourceThread2, SourceThread3;

    const char *SourceMessage1 = "Criacao de Source 1 Iniciada";
    const char *SourceMessage2 = "Criacao de Source 2 Iniciada";
    const char *SourceMessage3 = "Criacao de Source 3 Iniciada";

    int SourceRet1;
    //SourceRet1 = pthread_create( &SourceThread1, NULL, Source_Thread1, (void*) SourceMessage1);
    if(SourceRet1)
    {
        cout << "Failed to Create Source Thread 1" << endl;
    }
    usleep(5000);

    int SourceRet2;
    //SourceRet2 = pthread_create( &SourceThread2, NULL, Source_Thread2, (void*) SourceMessage2);
    if(SourceRet2)
    {
        cout << "Failed to Create Source Thread 2" << endl;
    }

    usleep(5000);

    int SourceRet3;
    //SourceRet3 = pthread_create( &SourceThread3, NULL, Source_Thread3, (void*) SourceMessage3);
    if(SourceRet3)
    {
        cout << "Failed to Create Source Thread 3" << endl;
    }
    //---------------------------------------------------------------------------------------------
    //---------------------------------------------------------------------------------------------


    return a.exec();
}

