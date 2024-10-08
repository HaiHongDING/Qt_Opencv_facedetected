#ifndef CAMRA_H
#define CAMRA_H

#include <QWidget>
#include <opencv2/opencv.hpp>
#include <QTimer>
#include <iostream>
namespace Ui {
class camra;
}

class camra : public QWidget
{
    Q_OBJECT

public:
    explicit camra(int index=0,QWidget *parent = nullptr);
    void timerStart();
    ~camra();
private slots:
    void onTimeout();
private:
    Ui::camra *ui;
    cv::VideoCapture cap;
    QTimer* timer;
    cv::Mat frame;
    double fps;
    int index;
};

#endif // CAMRA_H
