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
#include "eigenmesh.h"
namespace Ui {
class MainWindow;
}
class MainWindow : public QMainWindow
{

    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    enum SaveMode{IMAGE,CLOUD,DEPTH};
private:
    void zedSetting(sl::Camera* zed);
    bool checkCameraId();
    void convertSDKParam(sl::CameraParameters param,Calibrator& calibrator);
    Ui::MainWindow *ui;
//    cv::VideoCapture leftCapture;
//    cv::VideoCapture middleCapture;
//    cv::VideoCapture rightCapture;
    sl::Camera*leftZed;
    sl::Camera*middleZed;
    sl::Camera*rightZed;

    CalibrateResult* left2middle;
    CalibrateResult* right2middle;


    bool leftStopSignal;
    bool middleStopSignal;
    bool rightStopSignal;
    sl::InitParameters initParams;
    sl::RuntimeParameters runtime_parameters;
    SaveMode saveMode;
    bool useSDKParam;
    bool useRectified;
//    cv::Mat leftSbSResult;
//    cv::Mat middleSbSResult;
//    cv::Mat rightSbSResult;
//    cv::Mat leftFrame;
//    cv::Mat middleFrame;
//    cv::Mat rightFrame;
    QTimer *leftTimer;
    QTimer *middleTimer;
    QTimer *rightTimer;
    QGraphicsScene* leftScene;
    QGraphicsScene* middleScene;
    QGraphicsScene* rightScene;
    cv::Rect leftRect;
    cv::Rect rightRect;
    cv::Mat left;
    cv::Mat middle;
    cv::Mat right;
    cv::Mat leftDrawMat;
    cv::Mat middleDrawMat;
    cv::Mat rightDrawMat;
    cv::Mat leftGrayMat;
    cv::Mat middleGrayMat;
    cv::Mat rightGrayMat;

    cv::Mat leftDepth;
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
    int leftNum;
    int middleNum;
    int rightNum;

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
    void saveLeft(QString imageDir,QString drawDir,QString outputDir);
    void saveMiddle(QString imageDir,QString drawDir,QString outputDir);
    void saveRight(QString imageDir,QString drawDir,QString outputDir);
    bool readZedData(sl::Camera* zed,cv::Mat& out,SaveMode mode,bool wait=false);
    bool readZedImage(sl::Camera* zed,cv::Mat& image,bool wait=false);
    bool readZedDepth(sl::Camera* zed,cv::Mat& depth,bool wait=false);
    bool readZedCloud(sl::Camera* zed,cv::Mat& cloud,bool wait=false);
    bool detectAndDrawCorners(cv::Mat& image,cv::Mat& grayImage,cv::Mat& drawImage);
    void saveDepth(cv::Mat depth,std::string savePath);
    void saveCloud(cv::Mat cloud,EigenMesh& mesh,std::string savePath);
    void applyRigidTransform(Eigen::Matrix3Xf& cloud,CalibrateResult relative);
    void convertEigenMesh(const cv::Mat& cloud,EigenMesh& mesh);
private slots:
    void openCamera();
    void readCamera();
    void readLeftCamera();
    void readMiddleCamera();
    void readRightCamera();
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
};

#endif // MAINWINDOW_H
