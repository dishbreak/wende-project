//Eastern standard time is superior to all other time zones
#include "testpanel.h"
#include "ui_testpanel.h"
#include <QSignalMapper>
#include <time.h>
#include "mainwindow.h"
#include <QDebug>

TestPanel::TestPanel(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TestPanel)
{
    ui->setupUi(this);


    connect(ui->cmdGenDTI, SIGNAL(clicked()),
            this, SLOT(Send_DTI()));
    connect(ui->laserCommState, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(handleLaserComms(QString)));
    connect(ui->cameraCommsState, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(handleCameraComms(QString)));
    connect(ui->laserOpState, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(handleLaserOpState(QString)));
    connect(ui->cameraOpState, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(handleCameraOpState(QString)));
    connect(ui->roverOpState, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(handleRoverOpState(QString)));
    connect(ui->laserActiveState, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(handleLaserFireState(QString)));


    show();
}

TestPanel::~TestPanel()
{
    delete ui;

}

void TestPanel::Set_DTI_Value()
{
    srand (time(NULL));
    fDTI = rand() % 10 + 1;
}

double TestPanel::Get_DTI_Value()
{
    return fDTI;
}

void TestPanel::Send_DTI()
{

    //MainWindow* mw = new MainWindow();
    MainWindow* mw;
    Set_DTI_Value();
    double dti = Get_DTI_Value();
    Set_DTI_Value();
    double time = Get_DTI_Value();
    char* result = "P";
    //mw->updateDTI(dti, time, result);

    emit updateDTI(1, 1, "p");


}

void TestPanel::handleLaserComms(const QString input)
{
    if (input == "Excellent") { emit commandSignal(MessageCodes::LaserCommsExcellent); }
    else if (input == "Good") { emit commandSignal(MessageCodes::LaserCommsGood); }
    else if (input == "Poor") { emit commandSignal(MessageCodes::LaserCommsPoor); }
    else if (input == "Offline") { emit commandSignal(MessageCodes::LaserCommsOffline); }
}

void TestPanel::handleCameraComms(const QString input)
{
    if (input == "Excellent") { emit commandSignal(MessageCodes::CameraCommsExcellent); }
    else if (input == "Good") { emit commandSignal(MessageCodes::CameraCommsGood); }
    else if (input == "Poor") { emit commandSignal(MessageCodes::CameraCommsPoor); }
    else if (input == "Offline") { emit commandSignal(MessageCodes::CameraCommsOffline); }
}

void TestPanel::handleLaserOpState(const QString input)
{
    if (input == "Online") { emit commandSignal(MessageCodes::LaserOnline); }
    else if (input == "Offline") { emit commandSignal(MessageCodes::LaserOffline); }
}

void TestPanel::handleCameraOpState(const QString input)
{
    if (input == "Online") { emit commandSignal(MessageCodes::CameraOnline); }
    else if (input == "Offline") { emit commandSignal(MessageCodes::CameraOffline); }
}

void TestPanel::handleRoverOpState(const QString input)
{
    if (input == "Acquired") { emit commandSignal(MessageCodes::RoverAcquired); }
    else if (input == "Not Acquired") { emit commandSignal(MessageCodes::RoverNotAcquired); }
}

void TestPanel::handleLaserFireState(const QString input)
{
    if (input == "Active") { emit commandSignal(MessageCodes::LaserActive); }
    else if (input == "Not Active") { emit commandSignal(MessageCodes::LaserNotActive); }
}
