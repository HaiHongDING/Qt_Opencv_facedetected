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
    explicit camra(QWidget *parent = nullptr);
    void timerStart();
    ~camra();
private slots:
    void onTimeout();
private:
    Ui::camra *ui;
    cv::VideoCapture cap;
    QTimer* timer;
    cv::Mat frame;

};

#endif // CAMRA_H
