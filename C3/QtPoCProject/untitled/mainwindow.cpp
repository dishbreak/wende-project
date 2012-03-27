#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QMessageBox>
#include <QPushButton>
#include <QStatusBar>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QFileDialog>

#include "testpanel/testpanel.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    testPanel(new TestPanel(this))
{
    ui->setupUi(this);

    QTableWidget* tblDTI = new QTableWidget;
    QGroupBox* gbDTI = new QGroupBox(tr("DTI Log"));
    QVBoxLayout* vBox = new QVBoxLayout;
    QPushButton* vExport = new QPushButton(tr("Export"));

    connect(testPanel, SIGNAL(commandSignal(MessageCodes::StatusCode)),
            this, SLOT(updatePPI(MessageCodes::StatusCode)));

    connect(vExport, SIGNAL(clicked()),
            this, SLOT(openDialogue()));


    //Initialise Status Bar
    QFont font( "Arial" );
    font.setPointSize( 20 );
    statusBar()->setFont(font);
    statusBar()->showMessage("WENDE System Started...");



    //Set table row count 1 and column count 3
    tblDTI->setRowCount(3);
    tblDTI->setColumnCount(3);
    //tblDTI->

    tblDTI->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::MinimumExpanding);

    //Set Header Label Texts Here
    tblDTI->setHorizontalHeaderLabels(QString("DTI(m); TTI(s); PASS/FAIL").split(";"));

    //Add Table items here
    tblDTI->setItem(0,0,new QTableWidgetItem("1.45"));
    tblDTI->setItem(0,1,new QTableWidgetItem("18.0"));
    tblDTI->setItem(0,2,new QTableWidgetItem("PASS"));

    tblDTI->setItem(1,0,new QTableWidgetItem("1.09"));
    tblDTI->setItem(1,1,new QTableWidgetItem("12.0"));
    tblDTI->setItem(1,2,new QTableWidgetItem("PASS"));

    tblDTI->setItem(2,0,new QTableWidgetItem("2.10"));
    tblDTI->setItem(2,1,new QTableWidgetItem("30.0"));
    tblDTI->setItem(2,2,new QTableWidgetItem("FAIL"));

    //tblDTI->resizeColumnsToContents();


    vBox->addWidget((tblDTI));
    vBox->addWidget((vExport));

    gbDTI->setLayout(vBox);
    gbDTI->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    ui->vlDTI_2->addWidget(gbDTI);

    ui->gbLiveImage->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

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

void MainWindow::openDialogue()
{
   QString fileName = QFileDialog::getOpenFileName(this, tr("Export File"), "/", tr("Text Files (*.txt"));
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
        statusBar()->showMessage("Laser Communication POOR");
        break;
    case MessageCodes::LaserCommsOffline:
        //Set Laser Comms Image to "Offline"
        ui->widLaserComms->setPixmap(QPixmap(":/indicators/offlineIndicator.svg"));
        //Set LaserCommsOk to false
        laserCommsOk = false;
        statusBar()->showMessage("Laser Communication OFFLINE");
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
        statusBar()->showMessage("Laser OFFLINE");
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
        statusBar()->showMessage("Camera Communications POOR");
        break;
    case MessageCodes::CameraCommsOffline:
        //Set Camera Comms Image to "Offline"
        ui->widCameraComms->setPixmap(QPixmap(":/indicators/offlineIndicator.svg"));
        //Set CameraCommsOk to false
        cameraCommsOk = false;
        //Set Camera State to Unknown
        updatePPI(MessageCodes::CameraUnknown);
        statusBar()->showMessage("Camera Communication OFFLINE");
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
        ui->widRoverStatus->setPixmap(QPixmap(":/indicators/unknownIndicatorRover.svg"));
        break;
    case MessageCodes::LaserActive:
        //Set Laser Active Image to "Active"
        ui->widLaserActive->setPixmap(QPixmap(":/indicators/activeIndicator.svg"));
        break;
    case MessageCodes::LaserNotActive:
        //Set Laser Active Image to "Not Active"
        ui->widLaserActive->setPixmap(QPixmap(":/indicators/inactiveIndicator.svg"));
        break;
    case MessageCodes::wendeOnline:
        //Set WENDE Status Image to "Online"
        ui->widWendeStatus->setPixmap(QPixmap(":/indicators/goodIndicator.svg"));
        break;
    case MessageCodes::wendeOffline:
        //Set WENDE Status Image to "Offline"
        ui->widWendeStatus->setPixmap(QPixmap(":/indicators/offlineIndicator.svg"));
        statusBar()->showMessage("WENDE OFFLINE");
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
