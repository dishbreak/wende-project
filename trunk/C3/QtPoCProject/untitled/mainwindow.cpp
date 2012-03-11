#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QMessageBox>
#include <QPushButton>

#include "testpanel/testpanel.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    testPanel(new TestPanel(this))
{
    ui->setupUi(this);
    connect(testPanel, SIGNAL(commandSignal(MessageCodes::StatusCode)),
            this, SLOT(updatePPI(MessageCodes::StatusCode)));

    //set all bools to initial state
    wendeOperational = true;
    laserOk = true;
    cameraOk = true;
    laserCommsOk = true;
    cameraCommsOk = true;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updatePPI(const MessageCodes::StatusCode &status)
{
    qDebug() << "Got status code" << status;
    switch(status)
    {
    case MessageCodes::LaserCommsExcellent:
        //Set Laser Comms Image to "Excellent"
        ui->widLaserComms->setPixmap(QPixmap(":/indicators/goodIndicator.svg"));
        //Set LaserCommsOk to true
        laserCommsOk = true;
        break;
    case MessageCodes::LaserCommsGood:
        //Set Laser Comms Image to "Good"
        ui->widLaserComms->setPixmap(QPixmap(":/indicators/goodIndicator.svg"));
        //Set LaserCommsOk to true
        laserCommsOk = true;
        break;
    case MessageCodes::LaserCommsPoor:
        //Set Laser Comms Image to "Poor"
        ui->widLaserComms->setPixmap(QPixmap(":/indicators/goodIndicator.svg"));
        //Set LaserCommsOk to true
        laserCommsOk = true;
        break;
    case MessageCodes::LaserCommsOffline:
        //Set Laser Comms Image to "Offline"
        ui->widLaserComms->setPixmap(QPixmap(":/indicators/offlineIndicator.svg"));
        //Set LaserCommsOk to false
        laserCommsOk = false;
        //Set Laser State to Unknown
        updatePPI(MessageCodes::LaserUnknown);
        break;
    case MessageCodes::LaserOnline:
        if (laserCommsOk)
        {
            //Set Laser Status Image to "Online" (if Comms are up)
            ui->widLaserStatus->setPixmap(QPixmap(":/indicators/goodIndicator.svg"));
            //Set LaserOk to true
            laserOk = true;
        }
        else
        {
            qDebug() << "Can't say Laser is okay if comms are offline.";
        }
        break;
    case MessageCodes::LaserOffline:
        //Set Laser Status Image to "Offline"
        ui->widLaserStatus->setPixmap(QPixmap(":/indicators/offlineIndicator.svg"));
        //Set LaserOk to false
        laserOk = false;
        break;
    case MessageCodes::LaserUnknown:
        //Set Laser Status Image to "Unknown"
        ui->widLaserStatus->setPixmap(QPixmap(":/indicators/unknownIndicator.svg"));
        //Set LaserOk to false
        laserOk = false;
        break;
    case MessageCodes::CameraCommsExcellent:
        //Set Camera Comms Image to "Excellent"
        ui->widCameraComms->setPixmap(QPixmap(":/indicators/goodIndicator.svg"));
        //set CamaraCommsOk to true
        cameraCommsOk = true;
        break;
    case MessageCodes::CameraCommsGood:
        //Set Camera Comms Image to "Good"
        ui->widCameraComms->setPixmap(QPixmap(":/indicators/goodIndicator.svg"));
        //set CamaraCommsOk to true
        cameraCommsOk = true;
        break;
    case MessageCodes::CameraCommsPoor:
        //Set Camera Comms Image to "Poor"
        ui->widCameraComms->setPixmap(QPixmap(":/indicators/goodIndicator.svg"));
        //set CamaraCommsOk to true
        cameraCommsOk = true;
        break;
    case MessageCodes::CameraCommsOffline:
        //Set Camera Comms Image to "Offline"
        ui->widCameraComms->setPixmap(QPixmap(":/indicators/offlineIndicator.svg"));
        //Set CameraCommsOk to false
        cameraCommsOk = false;
        //Set Camera State to Unknown
        updatePPI(MessageCodes::CameraUnknown);
        break;
    case MessageCodes::CameraOnline:
        if (cameraCommsOk)
        {
            //Set Camera Status Image to "Online"
            ui->widCameraStatus->setPixmap(QPixmap(":/indicators/goodIndicator.svg"));
            //Set CameraOk to true
            cameraOk = true;
        }
        else
        {
            qDebug() << "Can't say Camera is okay if comms are offline.";
        }
        break;
    case MessageCodes::CameraOffline:
        //Set Camera Status Image to "Offline"
        ui->widCameraStatus->setPixmap(QPixmap(":/indicators/offlineIndicator.svg"));
        //Set CameraOk to false
        cameraOk = false;
        break;
    case MessageCodes::CameraUnknown:
        //Set Camera Status Image to "Unknown"
        ui->widCameraStatus->setPixmap(QPixmap(":/indicators/unknownIndicator.svg"));
        //Set CameraOk to false
        cameraOk = false;
        break;
    case MessageCodes::RoverAcquired:
        //Set Rover Status Image to "Acquired"
        ui->widRoverStatus->setPixmap(QPixmap(":/indicators/acquiredIndicator.svg"));
        break;
    case MessageCodes::RoverNotAcquired:
        //Set Rover Status Image to "Not Acquired"
        ui->widRoverStatus->setPixmap(QPixmap(":/indicators/unknownIndicator.svg"));
        break;
    case MessageCodes::LaserActive:
        //Set Laser Active Image to "Active"
        ui->widLaserActive->setPixmap(QPixmap(":/indicators/activeIndicator.svg"));
        break;
    case MessageCodes::LaserNotActive:
        //Set Laser Active Image to "Not Active"
        ui->widLaserActive->setPixmap(QPixmap("/indicators/inactiveIndicator.svg"));
        break;
    case MessageCodes::wendeOnline:
        //Set WENDE Status Image to "Online"
        ui->widWendeStatus->setPixmap(QPixmap(":/indicators/goodIndicator.svg"));
        break;
    case MessageCodes::wendeOffline:
        //Set WENDE Status Image to "Offline"
        ui->widWendeStatus->setPixmap(QPixmap(":/indicators/offlineIndicator.svg"));
        break;
    default:
        break;
    }
    //if LaserOp and CameraOp are both true, system is online
    if (laserOk && cameraOk)
    {
        if(!wendeOperational)
        {
            qDebug() << "Setting WENDE Operational";
            wendeOperational = true;
            updatePPI(MessageCodes::wendeOnline);
        }
    }
    //else, it's offline
    else
    {
        if(wendeOperational)
        {
            qDebug() << "It be broken. :-(";
            wendeOperational = false;
            updatePPI(MessageCodes::wendeOffline);
        }
    }
}
