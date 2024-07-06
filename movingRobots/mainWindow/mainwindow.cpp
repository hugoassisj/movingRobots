#include <QThread>
#include <iostream>
#include <pthread.h>
#include <qtimer.h>
#include "mainwindow.h"
#include "ui_mainwindow.h"

#define KEY_MOVEMENT 3

using namespace std;

mainWindow::mainWindow(QWidget *parent) :  QMainWindow(parent), ui(new Ui::mainWindow)
{
    ui->setupUi(this);
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(UIupdate()));
    timer->start(5);

    s1E = (bool)ui->checkBox->checkState();
    s2E = (bool)ui->checkBox_2->checkState();
    s3E = (bool)ui->checkBox_3->checkState();
    s4E = (bool)ui->checkBox_4->checkState();
    selected = 0;
}

void mainWindow::UIupdate()
{
    PosicionaRobos(r1,r2,r3);
    ShowPositions(r1, r2, r3);
    updateProgressBar(b1->getSize(),b1->getMaxSize());
}

void mainWindow::DefineRobos(Robot * robo1, Robot * robo2, Robot * robo3)
{
    r1 = robo1;
    r2 = robo2;
    r3 = robo3;
    Robots[0] = r1;
    Robots[1] = r2;
    Robots[2] = r3;

}

void mainWindow::DefineSources(Source * source1, Source * source2, Source * source3)
{
    s1 = source1;
    s2 = source2;
    s3 = source3;
}

void mainWindow::DefineBuffer(Buffer * buff)
{
    b1 = buff;
}

void mainWindow::DefineProcess(Process * process)
{
    p1 = process;
}

void mainWindow::PosicionaRobos(Robot * robo1, Robot * robo2, Robot * robo3)
{
    ui->robot1->move(robo1->getPosition().x,robo1->getPosition().y);
    ui->robot2->move(robo2->getPosition().x,robo2->getPosition().y);
    ui->robot3->move(robo3->getPosition().x,robo3->getPosition().y);
}

void mainWindow::ShowPositions(Robot * robo1, Robot * robo2, Robot * robo3)
{

    ui->rob1_x->setText(QString::number(robo1->getPosition().x + 10));
    ui->rob1_y->setText(QString::number(robo1->getPosition().y + 10));

    ui->rob2_x->setText(QString::number(robo2->getPosition().x + 10));
    ui->rob2_y->setText(QString::number(robo2->getPosition().y + 10));

    ui->rob3_x->setText(QString::number(robo3->getPosition().x + 10));
    ui->rob3_y->setText(QString::number(robo3->getPosition().y + 10));
}


mainWindow::~mainWindow()
{
    delete ui;
}

void mainWindow::on_robot1_clicked()
{
    selected = 0;
    this->setFocus();
}

void mainWindow::on_robot2_clicked()
{
    selected = 1;
    this->setFocus();
}

void mainWindow::on_robot3_clicked()
{
    selected = 2;
    this->setFocus();
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

void mainWindow::on_horizontalSlider_4_valueChanged(int value)
{
    ui->label_11->setText(QString::number(value));
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

int mainWindow::getSlider4Value()
{
    return ui->horizontalSlider_4->value();
}

void mainWindow::updateProgressBar(int size, int max)
{
    ui->max_l->setText(QString::number(max));
    ui->itens_l->setText(QString::number(size));
    ui->progressBar->setValue((int)(100 * size/ max));
}


void mainWindow::on_pushButton_clicked()
{
    Vector2D poss, x, newPos;
    int r;
    if (p1->available())
    {
        poss = b1->getPositions();
        x = poss.get();
        int linha = x.robotID;
        int coluna = x.sourceID;

        int l = p1->filterMatrixPosition(linha);
        int c = p1->filterMatrixPosition(coluna);

        p1->setProcess(l, c, poss);
        p1->display();
        newPos = p1->checkLine();
        r = newPos.get().robotID;
        switch (r) {
        case 0:
            r1->setPosition(newPos);
            break;
        case 1:
            r2->setPosition(newPos);
            break;
        case 2:
            r3->setPosition(newPos);
            break;
        default:
            cout << "nao rolou" << endl;
            break;
        }
        p1->display();
    }

}

void mainWindow::on_pushButton_2_clicked()
{
    //    b1->putPositions(x);
    //    x.y++;
    //    x.x++;
    //    x.sourceID++;
    //    x.robotID++;
}

void mainWindow::on_pushButton_3_clicked()
{
    b1->display();
    p1->display();
}

void mainWindow::on_checkBox_stateChanged(int arg1)
{
    s1E = (bool)ui->checkBox->checkState();
}


void mainWindow::on_checkBox_2_stateChanged(int arg1)
{
    s2E = (bool)ui->checkBox_2->checkState();
}

void mainWindow::on_checkBox_3_stateChanged(int arg1)
{
    s3E = (bool)ui->checkBox_3->checkState();
}

void mainWindow::on_checkBox_4_stateChanged(int arg1)
{
    s4E = (bool)ui->checkBox_4->checkState();
}

void mainWindow::keyPressEvent(QKeyEvent* ke)
{
    Vector2D pos;
    if (ke->key() == Qt::Key_Up)
    {
        pos = Robots[selected]->getPosition();
        pos.y = pos.y - KEY_MOVEMENT ;
        s1->Saturate(pos);
        Robots[selected]->setPosition(pos);
    }
    if (ke->key() == Qt::Key_Down)
    {
        pos = Robots[selected]->getPosition();
        pos.y = pos.y + KEY_MOVEMENT ;
        s1->Saturate(pos);
        Robots[selected]->setPosition(pos);
    }
    if (ke->key() == Qt::Key_Left)
    {
        pos = Robots[selected]->getPosition();
        pos.x = pos.x - KEY_MOVEMENT ;
        s1->Saturate(pos);
        Robots[selected]->setPosition(pos);
    }
    if (ke->key() == Qt::Key_Right)
    {
        pos = Robots[selected]->getPosition();
        pos.x = pos.x + KEY_MOVEMENT ;
        s1->Saturate(pos);
        Robots[selected]->setPosition(pos);
    }

}
