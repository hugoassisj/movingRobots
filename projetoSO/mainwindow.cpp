#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QThread>
#include <iostream>
#include <pthread.h>
#include <qtimer.h>

using namespace std;

void *CreateRobots( void *ptr )
{

}


mainWindow::mainWindow(QWidget *parent) :  QMainWindow(parent), ui(new Ui::mainWindow)
{
    ui->setupUi(this);
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(UIupdate()));
    timer->start(200);
}

   int i=1;
void mainWindow::UIupdate()
{
   PosicionaRobos(r1,r2,r3);
   ShowPositions(r1, r2, r3);
   updateProgressBar(i++,10); //b1->getSize(),b1->getMaxSize());
   if(i==10)i=1;
}

void mainWindow::DefineRobos(Robot &robo1, Robot &robo2, Robot &robo3)
{
    r1 = robo1;
    r2 = robo2;
    r3 = robo3;
}

void mainWindow::DefineBuffer(Buffer * buff)
{
    b1 = buff;
}

void mainWindow::PosicionaRobos(Robot &robo1, Robot &robo2, Robot &robo3)
{
    ui->robot1->move(robo1.getPosition().x,robo1.getPosition().y);
    ui->robot2->move(robo2.getPosition().x,robo2.getPosition().y);
    ui->robot3->move(robo3.getPosition().x,robo3.getPosition().y);
}

void mainWindow::ShowPositions(Robot &robo1, Robot &robo2, Robot &robo3)
{
    ui->rob1_x->setText(QString::number(robo1.getPosition().x/20 +1));
    ui->rob1_y->setText(QString::number(robo1.getPosition().y/20 +1));

    ui->rob2_x->setText(QString::number(robo2.getPosition().x/20 +1));
    ui->rob2_y->setText(QString::number(robo2.getPosition().y/20 +1));

    ui->rob3_x->setText(QString::number(robo3.getPosition().x/20 +1));
    ui->rob3_y->setText(QString::number(robo3.getPosition().y/20 +1));
}

void mainWindow::DefineSources(Source &source1, Source &source2, Source &source3)
{
//    s1 = source1;
//    s2 = source2;
//    s3 = source3;

//    s1.SetTime(ui->horizontalSlider->value());
//    s2.SetTime(ui->horizontalSlider_2->value());
//    s3.SetTime(ui->horizontalSlider_3->value());
}

mainWindow::~mainWindow()
{
    delete ui;
}

void mainWindow::on_robot1_clicked()
{
   Vector2D pos = r1.getPosition();
   pos.x+=20;
   pos.y+=20;
   if (pos.x > 380)
   {
       pos.x = pos.x - 380 - 20;
   }
   if (pos.y > 280)
   {
       pos.y = pos.y - 280 - 20;
   }
   r1.setPosition(pos);
}

void mainWindow::on_robot2_clicked()
{
    Vector2D pos = r2.getPosition();
    pos.x+=20;
    pos.y+=20;
    if (pos.x > 380)
    {
        pos.x = pos.x - 380 - 20;
    }
    if (pos.y > 280)
    {
        pos.y = pos.y - 280 - 20;
    }
    r2.setPosition(pos);
}

void mainWindow::on_robot3_clicked()
{
    Vector2D pos = r3.getPosition();
    pos.x+=20;
    pos.y+=20;
    if (pos.x > 380)
    {
        pos.x = pos.x - 380 - 20;
    }
    if (pos.y > 280)
    {
        pos.y = pos.y - 280 - 20;
    }
    r3.setPosition(pos);
}

void mainWindow::on_horizontalSlider_valueChanged(int value)
{
    ui->label_6->setText(QString::number(value));
}

void mainWindow::on_horizontalSlider_2_valueChanged(int value)
{
    ui->label_7->setText(QString::number(value));
}

void mainWindow::on_horizontalSlider_3_valueChanged(int value)
{
    ui->label_8->setText(QString::number(value));
}

int mainWindow::getSlider1Value()
{
    return ui->horizontalSlider->value();
}

int mainWindow::getSlider2Value()
{
   return ui->horizontalSlider_2->value();
}

int mainWindow::getSlider3Value()
{
    return ui->horizontalSlider_3->value();
}

void mainWindow::updateProgressBar(int size, int max)
{
    int val = 100* size/ max;
    ui->max_l->setText(QString::number(max));
    ui->itens_l->setText(QString::number(size));
    ui->progressBar->setValue((int)val);  
}
