#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "MessageCodes.h"
#include <QLabel>

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
    void openDialogue();
    
private:
    Ui::MainWindow *ui;
    TestPanel *testPanel;
    QLabel* lblStatus;
    QPalette* pal;
    bool wendeOperational;
    bool cameraOk;
    bool laserOk;
    bool cameraCommsOk;
    bool laserCommsOk;
};

#endif // MAINWINDOW_H
