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
    //added a line of code for the fun of it.
//    qDebug() << "Hello World! :-)";
//    QMessageBox::information(this, "Hello!", "I can make nifty dialog boxes! :-D", QMessageBox::Ok);
    connect(testPanel, SIGNAL(commandSignal(MessageCodes::StatusCode)),
            this, SLOT(updatePPI(MessageCodes::StatusCode)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updatePPI(const MessageCodes::StatusCode &status)
{
    switch(status)
    {
    case MessageCodes::LaserCommsExcellent:
        //Set Laser Comms Image to "Excellent"
        break;
    case MessageCodes::LaserCommsGood:
        //Set Laser Comms Image to "Good"
        break;
    case MessageCodes::LaserCommsPoor:
        //Set Laser Comms Image to "Poor"
        break;
    case MessageCodes::LaserCommsOffline:
        //Set Laser Comms Image to "Offline"
        //Set Laser State to Unknown
        break;
    case MessageCodes::LaserOnline:
        //Set Laser Status Image to "Online"
        //Set LaserOp to true
        break;
    case MessageCodes::LaserOffline:
        //Set Laser Status Image to "Offline"
        //Set LaserOp to false
        break;
    case MessageCodes::LaserUnknown:
        //Set Laser Status Image to "Unknown"
        break;
    case MessageCodes::CameraCommsExcellent:
        //Set Camera Comms Image to "Excellent"
        break;
    case MessageCodes::CameraCommsGood:
        //Set Camera Comms Image to "Good"
        break;
    case MessageCodes::CameraCommsPoor:
        //Set Camera Comms Image to "Poor"
        break;
    case MessageCodes::CameraCommsOffline:
        //Set Camera Comms Image to "Offline"
        //Set Camera State to Unknown
        break;
    case MessageCodes::CameraOnline:
        //Set Camera Status Image to "Online"
        //Set CameraOp to true
        break;
    case MessageCodes::CameraOffline:
        //Set Camera Status Image to "Offline"
        //Set CameraOp to false
        break;
    case MessageCodes::CameraUnknown:
        //Set Camera Status Image to "Unknown"
        //Set CameraOp to false
        break;
    case MessageCodes::RoverAcquired:
        //Set Rover Status Image to "Acquired"
        break;
    case MessageCodes::RoverNotAcquired:
        //Set Rover Status Image to "Not Acquired"
        break;
    case MessageCodes::LaserActive:
        //Set Laser Active Image to "Active"
        break;
    case MessageCodes::LaserNotActive:
        //Set Laser Active Image to "Not Active"
        break;
    case MessageCodes::wendeOnline:
    case MessageCodes::wendeOffline:
    default:
        break;
    }
    //if LaserOp and CameraOp are both true, system is online
    //else, it's offline
}
