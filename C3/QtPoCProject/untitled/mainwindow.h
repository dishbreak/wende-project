#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "MessageCodes.h"

#include <QTableWidget>
#include <QGroupBox>

namespace Ui {
class MainWindow;
}

//forward declarations
class TestPanel;

class MainWindow : public QMainWindow
{
    Q_OBJECT
    //test commit
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void updatePPI(const MessageCodes::StatusCode &);
    void updateDTI(double dti, double time, char * result);
    
private:
    Ui::MainWindow *ui;
    TestPanel *testPanel;
    bool wendeOperational;
    bool cameraOk;
    bool laserOk;
    bool cameraCommsOk;
    bool laserCommsOk;
    QTableWidget* tblDTI;
    QGroupBox* gbDTI;
};

#endif // MAINWINDOW_H
