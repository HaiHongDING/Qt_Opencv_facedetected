#ifndef CAMRA_H
#define CAMRA_H

#include <QWidget>
#include <opencv2/opencv.hpp>
#include <QTimer>
#include <QQueue>
#include <QImage>
#include <QDebug>
#include <QThread>
#include <QMutex>
#include <iostream>
using namespace std;
namespace Ui {
class camra;
class ImageQueue;
class FrameQueue;
class MyThread;
class ShowImageThread;
}
#define OPEN_DETECTED
#ifdef OPEN_DETECTED
class FrameQueue
{
public:
    FrameQueue() {}

    // 添加图像到队列
    void enqueue(const cv::Mat &image)
    {
        QMutexLocker locker(&mutex); // 锁住队列
        queue.enqueue(image);
        qDebug() << "Frame added to queue, current size:" << queue.size();
    }

    // 从队列中移除图像
    cv::Mat dequeue()
    {
        QMutexLocker locker(&mutex); // 锁住队列
        if (queue.isEmpty())
        {
            //qDebug() << "Queue is empty, returning a null frame.";
            return cv::Mat(); // 返回一个空的QImage
        }
        cv::Mat image = queue.dequeue();
        qDebug() << "Frame removed from queue, current size:" << queue.size();
        return image;
    }

    // 获取队列大小
    int size() const
    {
        QMutexLocker locker(&mutex); // 锁住队列
        return queue.size();
    }

private:
    QQueue<cv::Mat> queue; // 存储QImage的队列
    mutable QMutex mutex; // 互斥量，确保线程安全
};
class ImageQueue
{
public:
    ImageQueue() {}

    // 添加图像到队列
    void enqueue(const QPixmap &Pixmap)
    {
        QMutexLocker locker(&mutex); // 锁住队列
        queue.enqueue(Pixmap);
        qDebug() << "Image added to queue, current size:" << queue.size();
    }

    // 从队列中移除图像
    //QImage dequeue()
    QPixmap dequeue()
    {
        QMutexLocker locker(&mutex); // 锁住队列
        if (queue.isEmpty())
        {
            qDebug() << "Queue is empty, returning a null image.";
            return QPixmap(); // 返回一个空的QImage
        }
        QPixmap Pixmap = queue.dequeue();
        qDebug() << "Image removed from queue, current size:" << queue.size();
        return Pixmap;
    }

    // 获取队列大小
    int size() const
    {
        QMutexLocker locker(&mutex); // 锁住队列
        return queue.size();
    }

private:
    //QQueue<QImage> queue; // 存储QImage的队列
    QQueue<QPixmap> queue;
    mutable QMutex mutex; // 互斥量，确保线程安全
};
// 单例类，用于全局信号管理
class GlobalSignalManager : public QObject {
    Q_OBJECT

public:
    static GlobalSignalManager& instance() {
        static GlobalSignalManager instance;
        return instance;
    }

signals:
    void framesReady(); // 全局信号

private:
    GlobalSignalManager() {}
    GlobalSignalManager(const GlobalSignalManager&) = delete;
    GlobalSignalManager& operator=(const GlobalSignalManager&) = delete;
};
class MyThread : public QThread {
public:
    MyThread(ImageQueue &ImageQueue,FrameQueue &FrameQueue,cv::Size frameSize,QObject *parent = nullptr): \
        QThread(parent),frameSize(frameSize),FrameQueue(FrameQueue),ImageQueue(ImageQueue),stopFlag(false)
    {
        //std::string fd_model_path = "D:/Qt_Opencv_facedetected/lib/face_detection_yunet_2022mar.onnx";
        std::string fd_model_path = "../lib/face_detection_yunet_2022mar.onnx";//相对于cpp的位置
        // 创建人脸检测对象
        faceDetector = cv::FaceDetectorYN::create(fd_model_path, "", frameSize);
    }
    void stopThread()
    {
        stopFlag = true;
    }
protected:

    void run() override;

private:
    cv::Ptr<cv::FaceDetectorYN> faceDetector;
    cv::Size frameSize;
    FrameQueue &FrameQueue;
    ImageQueue &ImageQueue; 
    mutable QMutex mutex;
    std::atomic<bool> stopFlag;
};

class ShowImageThread : public QThread
{
public:
    ShowImageThread(ImageQueue &ImageQueue,QObject *parent = nullptr): \
        QThread(parent),ImageQueue(ImageQueue),stopFlag(false)
    {
    }
    void stopThread()
    {
        stopFlag = true;
    }
protected:

    void run() override;

private:
    ImageQueue &ImageQueue;
    mutable QMutex mutex;
    std::atomic<bool> stopFlag;
};
#endif
QImage MatImageToQt(const cv::Mat &src);
class camra : public QWidget
{
    Q_OBJECT

public:
    explicit camra(int index=0,QWidget *parent = nullptr);
    void timerStart();

    ~camra();
private slots:
    void onTimeout();
#ifdef OPEN_DETECTED
    void onTimeout2();

    void on_pushButton_clicked();

signals:
    void framesReady();
#endif
private:
    Ui::camra *ui;
    cv::VideoCapture cap;
    QTimer* timer;   
    cv::Mat frame;
    double fps;
    int index;
#ifdef OPEN_DETECTED
    QTimer* timer2;
    unsigned int ThreadNumber;
    cv::Size frameSize;
    ImageQueue ImageQueue;
    FrameQueue FrameQueue;
    ShowImageThread *ShowImageThread1;
    MyThread *MyThread1;
    MyThread *MyThread2;
    MyThread *MyThread3;
#endif
};

#endif // CAMRA_H
