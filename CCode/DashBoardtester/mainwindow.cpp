#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    SP = initialize(L"\\\\.\\COM10",9600);
    data_in = (SDE_in *)getDataInPT(SP);
    data_out = (SDE_out *)getDataOutPT(SP);
    
    // Creating a timer get status of DashBoard
    
      timer = new QTimer(this);
       connect(timer, SIGNAL(timeout()), this, SLOT(get_update()));
       timer->start(100);
      led_state = 0;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    float max_speed = 200;
    unsigned long speed = 1.0*value / ui->horizontalSlider->maximum()* max_speed;
    setSpeed(SP,speed);
    ui->speedText->setText(QString::number(speed));
}

void MainWindow::on_horizontalSlider_2_valueChanged(int value)
{
    float max_rmp = 8000;
    unsigned long rmp = 1.0*value / ui->horizontalSlider_2->maximum()* max_rmp;
    setRmp(SP,rmp);
    ui->rmpText->setText(QString::number(rmp));

}


void MainWindow::get_update() {
    unsigned long  speed;
    unsigned long  rmp;
    unsigned long  buttonState;
    unsigned long  acc, brake,clutch;
    getDisplayedSpeedRmp(SP,&speed,&rmp);
    getPedalStatus(SP, &acc, &brake, &clutch);
    getButtonState(SP, &buttonState);
    ui->button_state->setText(QString::number(buttonState));
    ui->Acc->setText(QString::number(acc));
    ui->Brake->setText(QString::number(brake));
    ui->Clutch->setText(QString::number(clutch));
    ui->speedCurrent->setText(QString::number(speed));
    ui->rmpCurrent->setText(QString::number(rmp));
    ui->buttons->setText(QString::number(buttonState));
}


void MainWindow::updateBit(bool state, int bit) {
    if (state) {
        bitSet(led_state,bit);
    } else {
        bitClear(led_state,bit);
    }
    ui->sendVal->setText(QString::number(led_state));
    setIndicators(SP,led_state);
}

void MainWindow::on_checkBox_2_stateChanged(int arg1)
{
    updateBit(arg1 > 0,0);
}

void MainWindow::on_checkBox_3_stateChanged(int arg1)
{
    updateBit(arg1 > 0,1);
}

void MainWindow::on_checkBox_4_stateChanged(int arg1)
{
    updateBit(arg1 > 0,2);
}

void MainWindow::on_checkBox_5_stateChanged(int arg1)
{
    updateBit(arg1 > 0,3);
}

void MainWindow::on_checkBox_6_stateChanged(int arg1)
{
    updateBit(arg1 > 0,4);
}

void MainWindow::on_checkBox_7_stateChanged(int arg1)
{
    updateBit(arg1 > 0,5);
}

void MainWindow::on_checkBox_8_stateChanged(int arg1)
{
    updateBit(arg1 > 0,6);
}

void MainWindow::on_checkBox_9_stateChanged(int arg1)
{
    updateBit(arg1 > 0,7);
}

void MainWindow::on_checkBox_stateChanged(int arg1)
{
updateBit(arg1 > 0,8);
}

void MainWindow::on_checkBox_10_stateChanged(int arg1)
{
updateBit(arg1 > 0,9);
}

void MainWindow::on_checkBox_11_stateChanged(int arg1)
{
updateBit(arg1 > 0,10);
}

void MainWindow::on_checkBox_12_stateChanged(int arg1)
{
    updateBit(arg1 > 0,11);
}

void MainWindow::on_checkBox_13_stateChanged(int arg1)
{
    updateBit(arg1 > 0,12);
}

void MainWindow::on_checkBox_14_stateChanged(int arg1)
{
    updateBit(arg1 > 0,13);
}

void MainWindow::on_checkBox_15_stateChanged(int arg1)
{
    updateBit(arg1 > 0,14);
}

void MainWindow::on_checkBox_16_stateChanged(int arg1)
{
    updateBit(arg1 > 0,15);
}

void MainWindow::on_checkBox_17_stateChanged(int arg1)
{
    updateBit(arg1 > 0,16);
}

void MainWindow::on_checkBox_18_stateChanged(int arg1)
{
    updateBit(arg1 > 0,17);
}


void MainWindow::on_SomeGear_stateChanged(int arg1)
{
    updateBit(arg1 > 0,18);
}


void MainWindow::on_Volant_stateChanged(int arg1)
{
    updateBit(arg1 > 0,19);
}


void MainWindow::on_Refuel_stateChanged(int arg1)
{
    updateBit(arg1 > 0,20);
}



