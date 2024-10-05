#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
    delete camra1;
}


void MainWindow::on_pushButton_clicked()
{
    camra1=new camra(this);
    camra1->show();
    camra1->timerStart();
}

