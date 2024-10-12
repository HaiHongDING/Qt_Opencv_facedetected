#include "camra.h"
#include "ui_camra.h"
#include <QPainter>
#include <QThread>
#include <vector>
#include <opencv2/objdetect.hpp>
using namespace std;
#define ISOpenDebug 0
#if ISOpenDebug
#define DebugLog do {   \
    std::cout << "DEBUG: " << __LINE__ << std::endl; \
} while(0)
#else
#define DebugLog //nothing
#endif

using namespace cv;
camra::camra(int index,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::camra)
{
    ui->setupUi(this);

    fps = 30;
    setWindowTitle("Display Image");
    resize(1280, 780); // 设置窗口大小
    cap.open(index);
    cap.set(CAP_PROP_FPS, fps);
    cap.set(CAP_PROP_FOURCC,VideoWriter::fourcc('M', 'J', 'P', 'G'));
    cap.set(cv::CAP_PROP_FRAME_WIDTH,1280);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT,720);
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(onTimeout()));
#ifdef OPEN_DETECTED
    ThreadNumber=2;
    timer2 = new QTimer(this);
    //connect(this, &camra::framesReady, this, &camra::onTimeout2);
    connect(&GlobalSignalManager::instance(), &GlobalSignalManager::framesReady, this, &camra::onTimeout2);
    //connect(timer2, SIGNAL(timeout()), this, SLOT(onTimeout2()));
    cap>>frame;
    frameSize=cv::Size(frame.cols, frame.rows);
    //myThread =new  MyThread[ThreadNumber](ImageQueue,FrameQueue,frameSize);
    MyThread1=new MyThread(ImageQueue,FrameQueue,frameSize);
    MyThread2=new MyThread(ImageQueue,FrameQueue,frameSize);
    MyThread3=new MyThread(ImageQueue,FrameQueue,frameSize);
#endif
    this->timerStart();
}
QImage MatImageToQt(const Mat &src)//判断Mat对象的类型并进行相应的转化
{
    if(src.type() == CV_8UC1)
    {
        cout<<"src.type() == CV_8UC1"<<endl;
        QImage qImage(src.cols,src.rows,QImage::Format_Indexed8);
        qImage.setColorCount(256);
        for(int i = 0; i < 256; i ++)
        {
            qImage.setColor(i,qRgb(i,i,i));
        }
        return qImage;
    }
    else if(src.type() == CV_8UC3)
    {
        const uchar *pSrc = (const uchar*)src.data;
        QImage qImage(pSrc,src.cols,src.rows,src.step,QImage::Format_RGB888);
        cout<<"src.type() == CV_8UC3"<<endl;
        return qImage.rgbSwapped();
    }
    else if(src.type() == CV_8UC4)
    {
        cout<<"src.type() == CV_8UC4"<<endl;
        const uchar *pSrc = (const uchar*)src.data;
        QImage qImage(pSrc, src.cols, src.rows, src.step, QImage::Format_ARGB32);
        return qImage.copy();
    }
    else
    {
        cout<<"src.type() == else"<<endl;
        return QImage();
    }
}
#ifdef OPEN_DETECTED
void camra::onTimeout2()//每一帧从QPixmap队列里拿出一个QPixmap对象显示在label上
{
    QPixmap qimage=ImageQueue.dequeue();
    if(!qimage.isNull())
    {
       ui->label->setPixmap(qimage);
    }
}
void MyThread:: run()//线程处理：将Mat队列中的原始图像拿出一个对象并转化成QPixmap对象入队QPixmap队列
{
    while(!stopFlag)
    {

        QMutexLocker locker(&mutex);
        cv::Mat faces;
        cv::Mat frame=FrameQueue.dequeue();
        locker.unlock();

        if(!frame.empty())
        {
            int x,y,w,h;
            faceDetector->detect(frame, faces);
            if(!faces.empty())
            {
                cout<<"faces.size():"<<faces.size()<<endl;
                x = faces.at<float>(0);
                y = faces.at<float>(1);
                w = faces.at<float>(2);
                h = faces.at<float>(3);
                //cv::rectangle(frame, cv::Rect(x,y,w,h), cv::Scalar(0,255,255));
               int lefteye_x=faces.at<float>(4);
               int lefteye_y=faces.at<float>(5);
               int righteye_x=faces.at<float>(6);
               int righteye_y=faces.at<float>(7);
               int nose_x=faces.at<float>(8);
               int nose_y=faces.at<float>(9);
               int leftLip_x=faces.at<float>(10);
               int leftLip_y=faces.at<float>(11);
               int rightLip_x=faces.at<float>(12);
               int rightLip_y=faces.at<float>(13);

               cv::rectangle(frame, cv::Rect(x,y,w,h), cv::Scalar(0,255,255));//脸所在矩形
               cv::circle(frame,cv::Point(lefteye_x,lefteye_y),5,cv::Scalar(0,255,255));//左眼圆
               cv::circle(frame,cv::Point(righteye_x,righteye_y),5,cv::Scalar(0,255,255));//右眼圆
               cv::circle(frame,cv::Point(nose_x,nose_y),5,cv::Scalar(0,255,255));//鼻子圆
               cv::circle(frame,cv::Point(leftLip_x,leftLip_y),5,cv::Scalar(0,255,255));//左嘴唇圆
               cv::circle(frame,cv::Point(rightLip_x,rightLip_y),5,cv::Scalar(0,255,255));//右嘴唇圆
               cv::line(frame,cv::Point(lefteye_x,lefteye_y),cv::Point(righteye_x,righteye_y),cv::Scalar(0,255,255));//左眼连右眼直线
               cv::line(frame,cv::Point(lefteye_x,lefteye_y),cv::Point(nose_x,nose_y),cv::Scalar(0,255,255));//左眼连鼻子直线
               cv::line(frame,cv::Point(righteye_x,righteye_y),cv::Point(nose_x,nose_y),cv::Scalar(0,255,255));//右眼连鼻子直线
               cv::line(frame,cv::Point(leftLip_x,leftLip_y),cv::Point(nose_x,nose_y),cv::Scalar(0,255,255));//左嘴唇连鼻子直线
               cv::line(frame,cv::Point(rightLip_x,rightLip_y),cv::Point(nose_x,nose_y),cv::Scalar(0,255,255));//右嘴唇连鼻子直线


            }
            locker.relock();
            QImage qImage=MatImageToQt(frame);
            if(!faces.empty())
            {
                QPainter painter(&qImage);
                QFont font("SimSun", 20); // 使用宋体或其他支持汉字的字体
                painter.setFont(font);
                painter.setPen(QColor(Qt::yellow));
                painter.drawText(x+w/2, y, QString::fromStdString("脸"));
            }
            ImageQueue.enqueue(QPixmap::fromImage(qImage));
            emit GlobalSignalManager::instance().framesReady();
            locker.unlock();
        }

    }
}
#endif
void camra::onTimeout()//每一帧摄像头采集一次图像放入Mat队列中
{

    cv::Mat frame;
    cap>>frame;
#ifdef OPEN_DETECTED
    FrameQueue.enqueue(frame);
#else
    ui->label->setPixmap(QPixmap::fromImage(MatImageToQt(frame)));
#endif
}

void camra::timerStart()
{
    ui->label->setScaledContents(true);
    timer->start(1000/fps);
#ifdef OPEN_DETECTED
    timer2->start(1000/fps);
    MyThread1->start();
    MyThread2->start();
    //MyThread3->start();
#endif
}


camra::~camra()
{
    delete ui;
    cap.release();
    timer->stop();
    delete timer;
#ifdef OPEN_DETECTED
    timer2->stop();
    MyThread1->stopThread();
    MyThread2->stopThread();
    MyThread3->stopThread();
    MyThread1->quit();
    MyThread2->quit();
    MyThread3->quit();
    delete timer2;
    delete MyThread1;
    delete MyThread2;
    delete MyThread3;
    deleteLater();
#endif
}

void camra::on_pushButton_clicked()
{
    timer->stop();
    timer2->stop();
    MyThread1->stopThread();
    MyThread2->stopThread();
    MyThread3->stopThread();
    MyThread1->quit();
    MyThread2->quit();
    MyThread3->quit();
}

