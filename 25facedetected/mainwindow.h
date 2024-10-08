#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "camra.h"
#include <QCameraInfo>
#include <QCamera>
//#include <QMediaDevices>
#include <QDebug>
#include <iostream>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
    camra *camra1;
    std::unordered_map<std::string, int> CamraMap;

};
#endif // MAINWINDOW_H
