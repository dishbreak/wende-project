#ifndef TESTPANEL_H
#define TESTPANEL_H

#include <QMainWindow>
#include "MessageCodes.h"

//forward declaration
class QSignalMapper;

namespace Ui {
class TestPanel;
}

class TestPanel : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit TestPanel(QWidget *parent = 0);
    ~TestPanel();
    QSignalMapper *signalMap;


signals:
    void commandSignal(const MessageCodes::StatusCode &);
    
private slots:
    void handleLaserComms(const QString);
    void handleCameraComms(const QString);
    void handleLaserOpState(const QString);
    void handleCameraOpState(const QString);
    void handleRoverOpState(const QString);
    void handleLaserFireState(const QString);

private:
    Ui::TestPanel *ui;
};

#endif // TESTPANEL_H
