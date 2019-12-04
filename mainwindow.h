#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <opencv2/opencv.hpp>
#include<QTimer>
#include <QGraphicsScene>
#include "settings.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <sl/Camera.hpp>
#include "utils/stereocalibrator.h"
#include <QThread>
#include <QTimer>
#include "zedcamerathread.h"
namespace Ui {
class MainWindow;
}
class MainWindow : public QMainWindow
{

    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void closeZed(sl::Camera**zed);
    void closeCameraThead(ZedCameraThread**t);
    void closeLeftCamera();
    void closeMiddleCamera();
    void closeRightCamera();
private:
    void zedSetting(sl::Camera* zed);
    bool checkCameraId();
    void convertSDKParam(sl::CameraParameters param,Calibrator& calibrator);
    Ui::MainWindow *ui;
    sl::Camera*leftZed;
    sl::Camera*middleZed;
    sl::Camera*rightZed;

    ZedCameraThread* leftZedCameraThread;
    ZedCameraThread* middleZedCameraThread;
    ZedCameraThread* rightZedCameraThread;
    ThreadSafeQueue<QPixmap>* leftShowQueue;
    ThreadSafeQueue<QPixmap>* middleShowQueue;
    ThreadSafeQueue<QPixmap>* rightShowQueue;

    CalibrateResult* left2middle;
    CalibrateResult* right2middle;


    bool leftStopSignal;
    bool middleStopSignal;
    bool rightStopSignal;
    sl::InitParameters initParams;
    sl::RuntimeParameters runtime_parameters;
    ZedCameraThread::SaveMode saveMode;
    bool useSDKParam;
    bool useRectified;
//    cv::Mat leftSbSResult;
//    cv::Mat middleSbSResult;
//    cv::Mat rightSbSResult;
//    cv::Mat leftFrame;
//    cv::Mat middleFrame;
//    cv::Mat rightFrame;
    QTime* leftTimer;
    QTime* middleTimer;
    QTime* rightTimer;
    QGraphicsScene* leftScene;
    QGraphicsScene* middleScene;
    QGraphicsScene* rightScene;
    cv::Rect leftRect;
    cv::Rect rightRect;

    cv::Mat middle;
    cv::Mat right;

    cv::Mat middleDrawMat;
    cv::Mat rightDrawMat;

    cv::Mat middleGrayMat;
    cv::Mat rightGrayMat;


    cv::Mat middleDepth;
    cv::Mat rightDepth;

    EigenMesh leftMesh;
    EigenMesh middleMesh;
    EigenMesh rightMesh;

    const int FRAME_WIDTH = 2560;
    const int FRAME_HEIGHT = 720;
    bool detect;
    bool check;
    bool capturing;

    float depthFactor;
    Settings& sets=Settings::instance();
    std::vector<cv::Mat> leftMats;
    std::vector<cv::Mat> leftGrayMats;
    std::vector<cv::Mat> middleMats;
    std::vector<cv::Mat> middleGrayMats;
    std::vector<cv::Mat> rightMats;
    std::vector<cv::Mat> rightGrayMats;
    //void openLeft(int leftId);
    void openLeftZed(int leftId);
//    void openMiddle(int middleId);
    void openMiddleZed(int middleId);
//    void openRight(int rightId);
    void openRightZed(int rightId);
    void openAll(int leftId,int middleId,int rightId);
    void closeAll();
    void saveLeft(QString imageDir,QString drawDir,QString outputDir);
    void saveMiddle(QString imageDir,QString drawDir,QString outputDir);
    void saveRight(QString imageDir,QString drawDir,QString outputDir);


    void applyRigidTransform(Eigen::Matrix3Xf& cloud,CalibrateResult relative);
    void convertEigenMesh(const cv::Mat& cloud,EigenMesh& mesh);
private slots:
    void openCamera();
    void closeCamera();
    void on_check_clicked();
    void on_settings_triggered();
    void on_save_clicked();
    void on_openLeft_clicked();
    void on_closeLeft_clicked();
    void on_openRight_clicked();
    void on_closeRight_clicked();
    void on_saveLeft_clicked();
    void on_saveRight_clicked();
    void on_saveMiddle_clicked();
    void on_openMiddle_clicked();
    void on_closeMiddle_clicked();
    void on_clearFile_triggered();
    void on_left2middle_clicked();
    void on_action_Image_triggered();
    void on_action_Cloud_triggered();
    void on_action_Depth_triggered();
    void on_mergeCloud_triggered();
    void on_right2middle_clicked();

    void showLeft();
    void showMiddle();
    void showRight();
signals:
    void closeLeftCameraThread();
    void closeMiddleCameraThread();
    void closeRightCameraThread();
};

#endif // MAINWINDOW_H
