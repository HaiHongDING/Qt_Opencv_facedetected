#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->pushButton->setEnabled(false);

}

MainWindow::~MainWindow()
{
    delete ui;
    delete camra1;
}


void MainWindow::on_pushButton_clicked()
{
    camra1=new camra(CamraMap.find(ui->comboBox->currentText().toStdString())->second,this);
    camra1->show();
    camra1->timerStart();
}


void MainWindow::on_pushButton_2_clicked()
{
    QList<QCameraInfo> cameras = QCameraInfo::availableCameras();

        if (cameras.isEmpty()) {
            qDebug() << "没有检测到摄像头。";
            return;
        }
        ui->comboBox->clear();
        for (int i=0;i<cameras.size();i++) {
            qDebug() << "摄像头位置:" << cameras.at(i).position();
            qDebug() << "摄像头索引:" << cameras.at(i).orientation();
            qDebug() << "摄像头名称:" << cameras.at(i).deviceName();
            qDebug() << "摄像头名字:" << cameras.at(i).description();
            ui->comboBox->addItem(cameras.at(i).description());
            qDebug() << "是否可用:" << (cameras.at(i).isNull() ? "否" : "是");
            CamraMap.insert(std::make_pair(cameras.at(i).description().toStdString(), i));
            // 这里可以添加其他信息，例如摄像头的描述等
        }
        auto cap=CamraMap.find(ui->comboBox->currentText().toStdString());
        if(cap!=CamraMap.end())
        {
            ui->pushButton->setEnabled(true);
        }
}

