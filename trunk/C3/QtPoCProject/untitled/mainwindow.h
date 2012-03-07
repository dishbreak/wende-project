#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "MessageCodes.h"

namespace Ui {
class MainWindow;
}

//forward declarations
class TestPanel;

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void updatePPI(const MessageCodes::StatusCode &);
    
private:
    Ui::MainWindow *ui;
    TestPanel *testPanel;
    bool wendeOperational;
    bool cameraOk;
    bool laserOk;
};

#endif // MAINWINDOW_H
