#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "robot.h"
#include "source.h"
#include "buffer.h"
#include "process.h"

namespace Ui { class mainWindow; }

class mainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit mainWindow(QWidget *parent = 0);
    void DefineRobos(Robot * robo1, Robot * robo2, Robot * robo3);
    void DefineBuffer(Buffer * buff);
    void DefineSources(Source * source1, Source * source2, Source * source3);
    void DefineProcess(Process * process);
    void PosicionaRobos(Robot * robo1, Robot * robo2, Robot * robo3);
    void ShowPositions(Robot * robo1, Robot * robo2, Robot * robo3);
    int getSlider1Value();
    int getSlider2Value();
    int getSlider3Value();
    void updateProgressBar(int size, int max);
    int s1E,s2E,s3E;

    ~mainWindow();

private slots:
    void on_robot1_clicked();

    void on_robot2_clicked();

    void on_robot3_clicked();

    void on_horizontalSlider_valueChanged(int value);

    void on_horizontalSlider_2_valueChanged(int value);

    void on_horizontalSlider_3_valueChanged(int value);

    void UIupdate();


    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_checkBox_stateChanged(int arg1);

private:
    Ui::mainWindow *ui;
    Robot * r1;
    Robot * r2;
    Robot * r3;
    Buffer * b1;
    Source * s1;
    Source * s2;
    Source * s3;
    Vector2D pos;
    Process * p1;

    Vector2D x;
};

#endif // MAINWINDOW_H
