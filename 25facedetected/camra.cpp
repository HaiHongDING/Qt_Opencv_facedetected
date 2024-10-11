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
    //cap.set(CAP_PROP_FPS, fps);
    cap.set(CAP_PROP_FOURCC,VideoWriter::fourcc('M', 'J', 'P', 'G'));
    cap.set(cv::CAP_PROP_FRAME_WIDTH,1280);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT,720);
    timer = new QTimer(this);
    //double  // 设置目标帧率
    cap>>frame;
    //std::string fd_model_path = "D:/Qt_Opencv_facedetected/lib/face_detection_yunet_2022mar.onnx";
    std::string fd_model_path = "../lib/face_detection_yunet_2022mar.onnx";//相对于cpp的位置
    // 创建人脸检测对象
    faceDetector = cv::FaceDetectorYN::create(fd_model_path, "", cv::Size(frame.cols, frame.rows));

    connect(timer, SIGNAL(timeout()), this, SLOT(onTimeout()));
}
QImage camra::MatImageToQt(const Mat &src)
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
        uchar *pSrc = src.data;
        for(int row = 0; row < src.rows; row ++)
        {
            uchar *pDest = qImage.scanLine(row);
            memcmp(pDest,pSrc,src.cols);
            pSrc += src.step;
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
void camra::onTimeout()
{
    cap>>frame;

    #if 0

    DebugLog;
    // 构造一个Mat对象，用来保存检测到的人脸位置信息
    cv::Mat faces;
    DebugLog;
    // 进行检测，检测的结果保存于faces中
    faceDetector->detect(frame, faces);
DebugLog;
// 打印输出人脸位置信息
    //cout << "faces: " << faces << endl;
#if 1
    if(!faces.empty())
    {
        //cout << "faces: " << faces << endl;
        // 使用矩形框标记人脸
        int x = faces.at<float>(0);
        //cout << faces.type() << endl; // CV_32F
        int y = faces.at<float>(1);
        int w = faces.at<float>(2);
        int h = faces.at<float>(3);
        DebugLog;
        cv::rectangle(frame, cv::Rect(x,y,w,h), cv::Scalar(0,255,255));
    }
#endif
//    for (size_t i = 0; i < faces.size(); i++)
//    {
//        rectangle(frame, faces[i], Scalar(255, 0, 0), 2); // 绘制矩形框
//    }
    DebugLog;

    #endif
    QImage qframe=MatImageToQt(frame);
    DebugLog;
    ui->label->setScaledContents(true);
    DebugLog;
    ui->label->setPixmap(QPixmap::fromImage(qframe));
    DebugLog;
}
void camra::timerStart()
{
    timer->start(1000/fps);
}
camra::~camra()
{
    delete ui;
    cap.release();
    timer->stop();
    delete timer;
}
