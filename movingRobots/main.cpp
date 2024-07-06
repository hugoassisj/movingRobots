#include <QApplication>
#include <QThread>
#include <pthread.h>
#include <unistd.h>
#include <iostream>

#include "mainWindow/mainwindow.h"
#include "Robot/robot.h"
#include "Buffer/buffer.h"
#include "Source/source.h"
#include "Processor/processor.h"


#define BUFFER_SIZE 300

using namespace std;


Robot r1 = Robot(0);
Robot r2 = Robot(1);
Robot r3 = Robot(2);

Robot * Robots[3] = {&r1, &r2, &r3};

Source s1 = Source(0);
Source s2 = Source(1);
Source s3 = Source(2);

Process p1;

mainWindow *UIptr;

Buffer b1(BUFFER_SIZE);



int robotID;
int sourceID;

Vector2D  new_pos[3];

int Time = 80000;


void showPos()
{

}

//Source_Thread1------------------------------------------------------------------------------------
void *Source_Thread1(void* ptr)
{
    //int Time = 20000;
    cout << (char *) ptr << endl;
    while (true)
    {
        if (UIptr->s1E)
        {
            //Source Produz uma nova Posição pra cada Robô
            for (int robot = 0; robot < 3; ++robot)
            {
                new_pos[robot].set(s1.produce(*Robots[robot]));
                b1.putPositions(new_pos[robot].get());
                usleep(rand() % Time);
            }

            //Mostra essas Novas Posições
            //            cout <<"Source: " << s1.getId() << "  -------------------------------------------------------------------"<< endl;
            //            for (int robot = 0; robot < 3; ++robot)
            //            {
            //                cout << "Robot: " << robot << "|X: " << new_pos[robot].get().x << "\t|Y: " << new_pos[robot].get().y << "\t|SourceID: " << new_pos[robot].get().sourceID << "\t|RobotID: " << new_pos[robot].get().robotID <<endl;
            //            }
            //            cout << endl;
        }

        usleep(UIptr->getSlider1Value()*1000);
    }
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------



//Source_Thread2------------------------------------------------------------------------------------
void *Source_Thread2(void* ptr)
{
    //int Time = 50000;
    cout << (char *) ptr << endl;
    while (true)
    {
        if (UIptr->s2E)
        {
            //Source Produz uma nova Posição pra cada Robô
            for (int robot = 0; robot < 3; ++robot)
            {
                new_pos[robot].set(s2.produce(*Robots[robot]));
                b1.putPositions(new_pos[robot].get());
                usleep(rand() % Time);
            }

            //Mostra essas Novas Posições
            //            cout <<"Source: " << s2.getId() << "  -------------------------------------------------------------------"<< endl;
            //            for (int robot = 0; robot < 3; ++robot)
            //            {
            //                cout << "Robot: " << robot << "|X: " << new_pos[robot].get().x << "\t|Y: " << new_pos[robot].get().y << "\t|SourceID: " << new_pos[robot].get().sourceID << "\t|RobotID: " << new_pos[robot].get().robotID <<endl;
            //            }
            //            cout << endl;
        }
        usleep(UIptr->getSlider2Value()*1000);
    }
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------



//Source_Thread3------------------------------------------------------------------------------------
void *Source_Thread3(void* ptr)
{
    //int Time = 80000;
    cout << (char *) ptr << endl;
    while (true)
    {
        if (UIptr->s3E)
        {
            //Source Produz uma nova Posição pra cada Robô
            for (int robot = 0; robot < 3; ++robot)
            {
                new_pos[robot].set(s3.produce(*Robots[robot]));
                b1.putPositions(new_pos[robot].get());
                usleep(rand() % Time);
            }

            //Mostra essas Novas Posições
            //            cout <<"Source: " << s3.getId() << "  -------------------------------------------------------------------"<< endl;
            //            for (int robot = 0; robot < 3; ++robot)
            //            {
            //                cout << "Robot: " << robot << "|X: " << new_pos[robot].get().x << "\t|Y: " << new_pos[robot].get().y << "\t|SourceID: " << new_pos[robot].get().sourceID << "\t|RobotID: " << new_pos[robot].get().robotID <<endl;
            //            }
            //            cout << endl;
        }
        usleep(UIptr->getSlider3Value()*1000);
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
        if (UIptr->s4E)
        {
            b1.display();
            Vector2D poss, x, newPos;
            int r;
            if (p1.available())
            {
                poss = b1.getPositions();
                x = poss.get();
                int linha = x.robotID;
                int coluna = x.sourceID;

                int l = p1.filterMatrixPosition(linha);
                int c = p1.filterMatrixPosition(coluna);

                p1.setProcess(l, c, poss);

                newPos = p1.checkLine();

                r = newPos.get().robotID;

                if (r >=0 && r < 3)
                {
                    Robots[r]->setPosition(newPos);
                }
            }
        }
        usleep(UIptr->getSlider4Value()*1000);
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
    w.DefineProcess(&p1);
    w.show();


    //---------------------------------------------------------------------------------------------
    //Cria Threads Source
    //---------------------------------------------------------------------------------------------
    pthread_t SourceThread1, SourceThread2, SourceThread3;

    const char *SourceMessage1 = "Criacao de Source 1 Iniciada";
    const char *SourceMessage2 = "Criacao de Source 2 Iniciada";
    const char *SourceMessage3 = "Criacao de Source 3 Iniciada";

    int SourceRet1;
    SourceRet1 = pthread_create( &SourceThread1, NULL, Source_Thread1, (void*) SourceMessage1);
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
    //---------------------------------------------------------------------------------------------
    //---------------------------------------------------------------------------------------------


    //---------------------------------------------------------------------------------------------
    //Cria Threads Processador
    //---------------------------------------------------------------------------------------------
    pthread_t ProcessThread;
    const char *ProcessMessage = "Criacao do Process Iniciada";
    int ProcessRet;
    ProcessRet = pthread_create( &ProcessThread, NULL, Process_Thread, (void*) ProcessMessage);
    if(ProcessRet)
    {
        cout << "Failed to Create Process Thread" << endl;
    }
    usleep(5000);
    //---------------------------------------------------------------------------------------------
    //---------------------------------------------------------------------------------------------


    return a.exec();
}

