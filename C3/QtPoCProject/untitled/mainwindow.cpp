#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //added a line of code for the fun of it.
    qDebug() << "Hello World! :-)";
    QMessageBox::information(this, "Hello!", "I can make nifty dialog boxes! :-D", QMessageBox::Ok);
}

MainWindow::~MainWindow()
{
    delete ui;
}
