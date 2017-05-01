#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "DashBoardDLL.h"
#include <QTimer>

#define bitSet(number,x) (number) |= 1 << (x);
#define bitClear(number,x) (number) &= ~(1 << (x));

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_horizontalSlider_valueChanged(int value);

    void on_horizontalSlider_2_valueChanged(int value);

private:
    Ui::MainWindow *ui;
    void * SP;
    SDE_in * data_in;
    SDE_out * data_out;
    QTimer *timer;
    unsigned long  led_state;
private slots:
    void get_update();
    void on_checkBox_2_stateChanged(int arg1);
    void on_checkBox_3_stateChanged(int arg1);
    void on_checkBox_4_stateChanged(int arg1);
    void on_checkBox_5_stateChanged(int arg1);
    void on_checkBox_6_stateChanged(int arg1);
    void on_checkBox_7_stateChanged(int arg1);
    void on_checkBox_8_stateChanged(int arg1);
    void on_checkBox_9_stateChanged(int arg1);
    void on_checkBox_stateChanged(int arg1);
    void on_checkBox_10_stateChanged(int arg1);
    void on_checkBox_11_stateChanged(int arg1);
    void updateBit(bool state, int bit);
    void on_checkBox_12_stateChanged(int arg1);
    void on_checkBox_13_stateChanged(int arg1);
    void on_checkBox_14_stateChanged(int arg1);
    void on_checkBox_15_stateChanged(int arg1);
    void on_checkBox_16_stateChanged(int arg1);
    void on_checkBox_17_stateChanged(int arg1);
    void on_checkBox_18_stateChanged(int arg1);
    void on_Refuel_stateChanged(int arg1);
    void on_Volant_stateChanged(int arg1);
    void on_SomeGear_stateChanged(int arg1);
};

#endif // MAINWINDOW_H
