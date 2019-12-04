#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QPixmap>
#include <QDir>
#include <QCameraInfo>
#include "dialog.h"

#include "qfilefunctions.h"
#include "modelwindow.h"
#include "process/pointcloudregister.h"
static int interval=90;
using namespace sl;
/**
 * @brief MainWindow::MainWindow
 * ————————————————
 *   版权声明：本文为CSDN博主「alan00000」的原创文章，遵循 CC 4.0 BY-SA 版权协议，转载请附上原文出处链接及本声明。
 *   原文链接：https://blog.csdn.net/alan00000/article/details/9836941
 */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    leftScene=new QGraphicsScene(this);
    middleScene=new QGraphicsScene(this);
    rightScene=new QGraphicsScene(this);
    ui->leftView->setScene(leftScene);
    ui->middleView->setScene(middleScene);
    ui->rightView->setScene(rightScene);
    ui->action_Cloud->setChecked(false);
    ui->action_Depth->setChecked(false);
    detect=false;
    capturing=false;

    leftTimer=new QTimer();
    leftTimer->start(interval);
    connect(leftTimer,&QTimer::timeout,this,&MainWindow::showLeft);
    connect(ui->open,SIGNAL(clicked()),this,SLOT(openCamera()));
    connect(ui->close,SIGNAL(clicked()),this,SLOT(closeCamera()));

    initParams.camera_resolution = RESOLUTION_HD1080;
    initParams.depth_mode = DEPTH_MODE_ULTRA;
//    initParams.coordinate_units = UNIT_METER;
    initParams.coordinate_units =UNIT_MILLIMETER;
    initParams.coordinate_system = COORDINATE_SYSTEM_IMAGE;
    runtime_parameters.sensing_mode = SENSING_MODE_STANDARD;
    depthFactor=1000;
    std::vector<sl::DeviceProperties> devList = sl::Camera::getDeviceList();
    int num = devList.size();
    std::cout << " Number of ZED Detected : " << num << std::endl;
    for(int i=0;i<num;i++){
        ui->leftId->addItem(QString::number(i),QVariant(i));
        ui->middleId->addItem(QString::number(i),QVariant(i));
        ui->rightId->addItem(QString::number(i),QVariant(i));
    }
    check=false;
    if(num==3){
        ui->leftId->setCurrentIndex(0);
        ui->middleId->setCurrentIndex(1);
        ui->rightId->setCurrentIndex(2);
        detect=true;
        check=true;
    }
    leftZed=nullptr;
    middleZed=nullptr;
    rightZed=nullptr;
    leftStopSignal=false;
    middleStopSignal=false;
    rightStopSignal=false;
    useSDKParam=true;
    useRectified=false;
    saveMode=ZedCameraThread::SaveMode::IMAGE;
    left2middle=nullptr;
    right2middle=nullptr;
    leftShowQueue=new ThreadSafeQueue<QPixmap>();
    middleShowQueue=new ThreadSafeQueue<QPixmap>();
    rightShowQueue=new ThreadSafeQueue<QPixmap>();
    leftZedCameraThread=nullptr;
    middleZedCameraThread=nullptr;
    rightZedCameraThread=nullptr;
}

MainWindow::~MainWindow()
{
    closeCamera();
    delete ui;
}

void MainWindow::zedSetting(sl::Camera* zed)
{
    zed->setCameraSettings(sl::CAMERA_SETTINGS_BRIGHTNESS, 4);
    zed->setCameraSettings(sl::CAMERA_SETTINGS_CONTRAST, 8);
    zed->setCameraSettings(sl::CAMERA_SETTINGS_HUE, 0);
    zed->setCameraSettings(sl::CAMERA_SETTINGS_SATURATION, 4);
    zed->setCameraSettings(sl::CAMERA_SETTINGS_WHITEBALANCE, 4600);
    zed->setCameraSettings(sl::CAMERA_SETTINGS_GAIN, 4);
    zed->setCameraSettings(sl::CAMERA_SETTINGS_EXPOSURE, 80);
    if(initParams.coordinate_units==UNIT_METER){
        zed->setDepthMaxRangeValue(1);
    }else if(initParams.coordinate_units==UNIT_MILLIMETER){
        zed->setDepthMaxRangeValue(1000);
    }

}

bool MainWindow::checkCameraId()
{
    if(!check)return  true;
    int leftId=ui->leftId->currentIndex();
    int middleId=ui->middleId->currentIndex();
    int rightId=ui->rightId->currentIndex();
    if(leftId==middleId||middleId==rightId||rightId==leftId){
        return false;
    }
    return true;
}

void MainWindow::convertSDKParam(CameraParameters param, Calibrator &calibrator)
{
    std::cout<<"convertSDKParam..."<<std::endl<<std::flush;
    CalibrateResult result;
//    result.instrisincMatrix=(cv::Mat_<double>()<<param.fx,0,param.cx,
//                             0,param.fy,param.cy,
//                             0,0,1);
    result.instrisincMatrix=cv::Mat::zeros(3,3,CV_64F);
    result.instrisincMatrix.at<double>(0,0)=param.fx;result.instrisincMatrix.at<double>(0,2)=param.cx;
    result.instrisincMatrix.at<double>(1,1)=param.fy;result.instrisincMatrix.at<double>(1,2)=param.cy;
    result.instrisincMatrix.at<double>(2,2)=1.0;
    std::cout<<"instrisincMatrix:"<<result.instrisincMatrix<<std::endl<<std::flush;
    result.distortionCoeff=cv::Mat::zeros(5,1,CV_64F);
    for(int i=0;i<5;i++){
        result.distortionCoeff.at<double>(i,0)=param.disto[i];
    }
    calibrator.setResult(result);
    std::cout<<"convertSDKParam done!"<<std::endl<<std::flush;
}

//void MainWindow::openLeft(int leftId)
//{

//    leftCapture.open(leftId);
//    if(!leftCapture.isOpened()){
//        qDebug()<<"open camera id:"<<leftId<<" fail!";
//    }
//    leftCapture.set(cv::CAP_PROP_FRAME_WIDTH, FRAME_WIDTH);
//    leftCapture.set(cv::CAP_PROP_FRAME_HEIGHT, FRAME_HEIGHT);
//}

void MainWindow::openLeftZed(int leftId)
{
    if(leftZed)return;
    leftZed=new sl::Camera();
    initParams.input.setFromCameraID(leftId);
    sl::ERROR_CODE err =leftZed->open(initParams);
    if (err != sl::SUCCESS) {
        std::cout << leftZed->getCameraInformation().camera_model << " n*" << leftId << " -> Result: " << sl::toString(err) << std::endl;
        delete leftZed;
        exit(EXIT_FAILURE);
    } else
        std::cout << leftZed->getCameraInformation().camera_model << " n*" << leftId << " SN " <<
                     leftZed->getCameraInformation().serial_number << " -> Result: " << sl::toString(err) << std::endl;
    zedSetting(leftZed);
    leftZedCameraThread=new ZedCameraThread();
    leftZedCameraThread->setId(leftId);
    leftZedCameraThread->setShowQueue(leftShowQueue);
    leftZedCameraThread->zed=leftZed;
    leftZedCameraThread->setSaveMode(saveMode);
    connect(leftZedCameraThread,&ZedCameraThread::returnQPixmap,this,&MainWindow::showLeft);
    connect(this,&MainWindow::closeLeftCameraThread,leftZedCameraThread,&ZedCameraThread::close);
    leftZedCameraThread->start();
    std::cout<<" openLeftZed done!"<<std::endl<<std::flush;
}

void MainWindow::openMiddleZed(int middleId)
{
    if(middleZed)return;
    middleZed=new sl::Camera();
    initParams.input.setFromCameraID(middleId);
    sl::ERROR_CODE err =middleZed->open(initParams);
    if (err != sl::SUCCESS) {
        std::cout << middleZed->getCameraInformation().camera_model << " n*" << middleId << " -> Result: " << sl::toString(err) << std::endl;
        delete middleZed;
        exit(EXIT_FAILURE);
    } else
        std::cout << middleZed->getCameraInformation().camera_model << " n*" << middleId << " SN " <<
                     middleZed->getCameraInformation().serial_number << " -> Result: " << sl::toString(err) << std::endl;
    zedSetting(middleZed);
    middleZedCameraThread=new ZedCameraThread();
    middleZedCameraThread->setId(middleId);
    middleZedCameraThread->setShowQueue(middleShowQueue);
    middleZedCameraThread->zed=middleZed;
    middleZedCameraThread->setSaveMode(saveMode);
    connect(middleZedCameraThread,&ZedCameraThread::returnQPixmap,this,&MainWindow::showMiddle);
    connect(this,&MainWindow::closeMiddleCameraThread,middleZedCameraThread,&ZedCameraThread::close);
    middleZedCameraThread->start();
    std::cout<<" openMiddleZed done!"<<std::endl<<std::flush;
}

void MainWindow::openRightZed(int rightId)
{
    if(rightZed)return;
    rightZed=new sl::Camera();
    initParams.input.setFromCameraID(rightId);
    sl::ERROR_CODE err =rightZed->open(initParams);
    if (err != sl::SUCCESS) {
        std::cout << rightZed->getCameraInformation().camera_model << " n*" << rightId << " -> Result: " << sl::toString(err) << std::endl;
        delete rightZed;
        exit(EXIT_FAILURE);
    } else
        std::cout << rightZed->getCameraInformation().camera_model << " n*" << rightId << " SN " <<
                     rightZed->getCameraInformation().serial_number << " -> Result: " << sl::toString(err) << std::endl;
    zedSetting(rightZed);
    rightZedCameraThread=new ZedCameraThread();
    rightZedCameraThread->setId(rightId);
    rightZedCameraThread->setShowQueue(rightShowQueue);
    rightZedCameraThread->zed=rightZed;
    rightZedCameraThread->setSaveMode(saveMode);
    connect(rightZedCameraThread,&ZedCameraThread::returnQPixmap,this,&MainWindow::showRight);
    connect(this,&MainWindow::closeRightCameraThread,rightZedCameraThread,&ZedCameraThread::close);
    rightZedCameraThread->start();
    std::cout<<" openRightZed done!"<<std::endl<<std::flush;
}

void MainWindow::openAll(int leftId,int middleId,int rightId)
{
    openLeftZed(leftId);
    openMiddleZed(middleId);
    openRightZed(rightId);
}

void MainWindow::saveLeft(QString imageDir,QString drawDir,QString outputDir)
{
    if(leftZedCameraThread)leftZedCameraThread->saveZedData(imageDir,drawDir,outputDir);
}

void MainWindow::saveMiddle(QString imageDir, QString drawDir,QString outputDir)
{
    if(middleZedCameraThread)middleZedCameraThread->saveZedData(imageDir,drawDir,outputDir);
}

void MainWindow::saveRight(QString imageDir,QString drawDir,QString outputDir)
{
    if(rightZedCameraThread)rightZedCameraThread->saveZedData(imageDir,drawDir,outputDir);
}


void MainWindow::applyRigidTransform(Eigen::Matrix3Xf &cloud, CalibrateResult relative)
{
    int i,j;
    cv::Mat R=relative.R;
    cv::Mat T=relative.T;
    Eigen::Matrix<float,3,3> RE=Eigen::Matrix<float,3,3>::Identity();
    Eigen::Matrix<float,3,1> TE=Eigen::Matrix<float,3,1>::Zero();
    for(i=0;i<3;i++){
        for(j=0;j<3;j++){
            RE(i,j)=R.at<double>(i,j);
        }
        TE(i,0)=T.at<double>(i,0);
    }
    std::cout<<"RE:"<<RE<<std::endl<<std::flush;
    std::cout<<"TE:"<<TE<<std::endl<<std::flush;
    cloud=RE*cloud;
    cloud.colwise()+=TE;
}

void MainWindow::convertEigenMesh(const cv::Mat& cloud, EigenMesh &mesh)
{
    Eigen::Matrix3Xf points;
    Eigen::Matrix4Xf colors;
    std::vector<Eigen::Vector3f> pv;
    std::vector<Eigen::Vector4f> cv;
    int i,j;
    for (i = 0; i < cloud.rows; i++) {
        for (j = 0; j < cloud.cols; j++) {
            cv::Vec4f f4=cloud.at<cv::Vec4f>(i,j);
            if (std::isnan(f4[0]) || std::isnan(f4[1]) || std::isnan(f4[2]))continue;
            if (std::isinf(f4[0]) || std::isinf(f4[1]) || std::isinf(f4[2]))continue;
            pv.push_back(Eigen::Vector3f(f4[0],f4[1],f4[2]));
            unsigned char* temp= (unsigned char*)&f4[3];
            cv.push_back(Eigen::Vector4f((int)temp[0],(int)temp[1],(int)temp[2],255));
        }
    }
    points.resize(3,pv.size());
    colors.resize(4,cv.size());
    for(i=0;i<pv.size();i++){
        points.col(i)=pv[i];
        colors.col(i)=cv[i];
    }
    mesh.setPoints(points);
    mesh.setColors(colors);
}

void MainWindow::openCamera()
{
    int leftId=ui->leftId->currentIndex();
    int middleId=ui->middleId->currentIndex();
    int rightId=ui->rightId->currentIndex();
    if(leftId==middleId||middleId==rightId||rightId==leftId){
        ui->statusBar->showMessage(tr("select the same camera!"), 2000);
        return;
    }
    openAll(leftId,middleId,rightId);
    ui->leftId->setDisabled(true);
    ui->middleId->setDisabled(true);
    ui->rightId->setDisabled(true);
}


void MainWindow::closeCamera()
{
    ui->leftId->setDisabled(false);
    ui->middleId->setDisabled(false);
    ui->rightId->setDisabled(false);
    leftScene->clear();
    middleScene->clear();
    rightScene->clear();
    closeAll();
}
void MainWindow::closeAll()
{
    closeLeftCamera();
    closeMiddleCamera();
    closeRightCamera();
}
void MainWindow::on_check_clicked()
{
    detect=!detect;
    if(leftZedCameraThread){
        leftZedCameraThread->setDetect(detect);
    }
    if(middleZedCameraThread){
        middleZedCameraThread->setDetect(detect);
    }
    if(rightZedCameraThread){
        rightZedCameraThread->setDetect(detect);
    }
}

void MainWindow::on_settings_triggered()
{
    Dialog dialog;
    dialog.exec();
}

void MainWindow::on_save_clicked()
{
    capturing=true;
    QString imageDir=sets.getImageDir();
    QString drawDir=sets.getDrawDir();
    QString outputDir=sets.getOutputDir();
    QDir tmp;
    tmp.mkpath(imageDir);
    tmp.mkpath(drawDir);

    saveLeft(imageDir,drawDir,outputDir);
    saveMiddle(imageDir,drawDir,outputDir);
    saveRight(imageDir,drawDir,outputDir);
    capturing=false;
}


void MainWindow::on_openLeft_clicked()
{
    if(!checkCameraId()){
        ui->statusBar->showMessage(tr("select the same camera!"), 2000);
        return;
    }
    //openLeft(ui->leftId->currentIndex());
    openLeftZed(ui->leftId->currentIndex());
    ui->leftId->setDisabled(true);

}

void MainWindow::closeZed(sl::Camera**zed)
{
    if(*zed){
        (*zed)->close();
        delete *zed;
        *zed=nullptr;
    }
}

void MainWindow::closeCameraThead(ZedCameraThread **t)
{
    (*t)->setStopSignal(true);
    (*t)->quit();
    (*t)->wait();
    delete (*t);
    (*t)=nullptr;
}

void MainWindow::closeLeftCamera()
{
    if(leftZedCameraThread){
        //emit closeLeftCameraThread();
        leftZedCameraThread->close();
        closeCameraThead(&leftZedCameraThread);
    }
    closeZed(&leftZed);
}

void MainWindow::closeMiddleCamera()
{
    if(middleZedCameraThread){
        //emit closeMiddleCameraThread();
        middleZedCameraThread->close();
        closeCameraThead(&middleZedCameraThread);
    }
    closeZed(&middleZed);
}

void MainWindow::closeRightCamera()
{
    if(rightZedCameraThread){
        //emit closeRightCameraThread();
        rightZedCameraThread->close();
        closeCameraThead(&rightZedCameraThread);
    }
    closeZed(&rightZed);
}

void MainWindow::on_closeLeft_clicked()
{
    ui->leftId->setDisabled(false);
    leftScene->clear();
    closeLeftCamera();
}

void MainWindow::on_openRight_clicked()
{
    if(!checkCameraId()){
        ui->statusBar->showMessage(tr("select the same camera!"), 2000);
        return;
    }
    //openRight(ui->rightId->currentIndex());
    openRightZed(ui->rightId->currentIndex());
    ui->rightId->setDisabled(true);
}

void MainWindow::on_closeRight_clicked()
{
    ui->rightId->setDisabled(false);
    rightScene->clear();
    closeRightCamera();
}

void MainWindow::on_saveLeft_clicked()
{
    QString imageDir=sets.getImageDir();
    QString drawDir=sets.getDrawDir();
    QString outputDir=sets.getOutputDir();
    QDir tmp;
    tmp.mkpath(imageDir);
    tmp.mkpath(drawDir);
    saveLeft(imageDir,drawDir,outputDir);
}

void MainWindow::on_saveRight_clicked()
{
    QString imageDir=sets.getImageDir();
    QString drawDir=sets.getDrawDir();
    QString outputDir=sets.getOutputDir();
    QDir tmp;
    tmp.mkpath(imageDir);
    tmp.mkpath(drawDir);
    saveRight(imageDir,drawDir,outputDir);
}

void MainWindow::on_saveMiddle_clicked()
{
    QString imageDir=sets.getImageDir();
    QString drawDir=sets.getDrawDir();
    QString outputDir=sets.getOutputDir();
    QDir tmp;
    tmp.mkpath(imageDir);
    tmp.mkpath(drawDir);
    saveMiddle(imageDir,drawDir,outputDir);
}

void MainWindow::on_openMiddle_clicked()
{
    if(!checkCameraId()){
        ui->statusBar->showMessage(tr("select the same camera!"), 2000);
        return;
    }
    //openMiddle(ui->middleId->currentIndex());
    openMiddleZed(ui->middleId->currentIndex());
    ui->middleId->setDisabled(true);
}

void MainWindow::on_closeMiddle_clicked()
{
    ui->middleId->setDisabled(false);
    middleScene->clear();
    closeMiddleCamera();
}


void MainWindow::on_clearFile_triggered()
{
    QFileFunctions::clearFolder(sets.getDrawDir(),false);
    QFileFunctions::clearFolder(sets.getImageDir(),false);
    QFileFunctions::clearFolder(sets.getOutputDir(),false);
}

void MainWindow::on_left2middle_clicked()
{
    ui->statusBar->showMessage(tr("calibrate two..."), 2000);
    detect=false;
    Calibrator srcCalibrator,dstCalibrator;
    StereoCalibrator stereoCalibrator;
    QString outputDir=sets.getOutputDir();
    QDir tmp;
    tmp.mkpath(outputDir);
    QString leftOutputFileName=outputDir+QDir::separator()+"left.yml";
    QString middleOutputFileName=outputDir+QDir::separator()+"middle.yml";
    QString relativeOutputFileName=outputDir+QDir::separator()+"left2middle.yml";
    std::cout<<"11111111111"<<std::endl<<std::flush;
    if(leftGrayMats.size()>0&&middleGrayMats.size()>0){
        srcCalibrator.setOutputFileName(leftOutputFileName.toStdString());
        srcCalibrator.setImages(leftMats);
        srcCalibrator.setGrayImages(leftGrayMats);
        srcCalibrator.setChessBoardConfig(sets.chessBoardConfig);

        dstCalibrator.setOutputFileName(middleOutputFileName.toStdString());
        dstCalibrator.setImages(middleMats);
        dstCalibrator.setGrayImages(middleGrayMats);
        dstCalibrator.setChessBoardConfig(sets.chessBoardConfig);
        //std::cout<<"*************"<<std::endl<<std::flush;

    }else{
        QString imageDir=sets.getImageDir();
        QString imageSuffix=sets.getImageSurffix();
        QDir dir(imageDir);
        if(!dir.exists())return;
        QString leftImageDir=imageDir+QDir::separator()+"left";
        QDir leftDir(leftImageDir);
        if(!leftDir.exists())return;
        QString middleImageDir=imageDir+QDir::separator()+"middle";
        QDir middleDir(middleImageDir);
        if(!middleDir.exists())return;

        leftDir.setFilter(QDir::Files);
        QFileInfoList leftFileList = leftDir.entryInfoList();
        int leftFileCount = leftFileList.count();

        middleDir.setFilter(QDir::Files);
        QFileInfoList middleFileList = middleDir.entryInfoList();
        int middleFileCount = middleFileList.count();
        if(middleFileCount==0||middleFileCount==0||leftFileCount!=middleFileCount)return;
        std::vector<cv::Mat> leftImgs;
        std::vector<cv::Mat> leftGrayImgs;

        std::vector<cv::Mat> middleImgs;
        std::vector<cv::Mat> middleGrayImgs;
        for(int i=0;i<leftFileCount;i++)
        {
            QFileInfo leftFileInfo = leftFileList[i];
            QString leftSuffix = leftFileInfo.suffix();
            if(QString::compare(leftSuffix, imageSuffix, Qt::CaseInsensitive) == 0) {
                QString filePath = leftFileInfo.absoluteFilePath();
                QString fileName = leftFileInfo.baseName();
                cv::Mat img=cv::imread(filePath.toStdString());
                leftImgs.push_back(img);
                cv::Mat gray;
                cvtColor(img,gray,CV_BGR2GRAY);
                leftGrayImgs.push_back(gray);
            }

            QFileInfo middleFileInfo = middleFileList[i];
            QString middleSuffix = middleFileInfo.suffix();
            if(QString::compare(middleSuffix, imageSuffix, Qt::CaseInsensitive) == 0) {
                QString filePath = middleFileInfo.absoluteFilePath();
                QString fileName = middleFileInfo.baseName();
                cv::Mat img=cv::imread(filePath.toStdString());
                middleImgs.push_back(img);
                cv::Mat gray;
                cvtColor(img,gray,CV_BGR2GRAY);
                middleGrayImgs.push_back(gray);
            }
        }

        if(leftImgs.size()!=middleImgs.size())return;
        std::cout<<"----------"<<std::endl<<std::flush;
        srcCalibrator.setOutputFileName(leftOutputFileName.toStdString());
        srcCalibrator.setImages(leftImgs);
        srcCalibrator.setGrayImages(leftGrayImgs);
        srcCalibrator.setChessBoardConfig(sets.chessBoardConfig);

        dstCalibrator.setOutputFileName(middleOutputFileName.toStdString());
        dstCalibrator.setImages(middleImgs);
        dstCalibrator.setGrayImages(middleGrayImgs);
        dstCalibrator.setChessBoardConfig(sets.chessBoardConfig);
    }
    //std::cout<<"2222222222"<<std::endl<<std::flush;
    if(useSDKParam){
        if(useRectified){
            convertSDKParam(leftZed->getCameraInformation().calibration_parameters.left_cam,srcCalibrator);
            srcCalibrator.setUseSDKParam(true);
            convertSDKParam(middleZed->getCameraInformation().calibration_parameters.left_cam,dstCalibrator);
            dstCalibrator.setUseSDKParam(true);
        }else{
            convertSDKParam(leftZed->getCameraInformation().calibration_parameters_raw.left_cam,srcCalibrator);
            srcCalibrator.setUseSDKParam(true);
            convertSDKParam(middleZed->getCameraInformation().calibration_parameters_raw.left_cam,dstCalibrator);
            dstCalibrator.setUseSDKParam(true);
        }

    }
    //std::cout<<"33333333"<<std::endl<<std::flush;
    stereoCalibrator.setSrcCalibrator(srcCalibrator);
    stereoCalibrator.setDstCalibrator(dstCalibrator);
    stereoCalibrator.setRelativeOutputFileName(relativeOutputFileName.toStdString());
    stereoCalibrator.calibrateStereo();
    left2middle=new CalibrateResult();
    *left2middle=stereoCalibrator.getRelative();
    if(initParams.coordinate_units == UNIT_METER){
        (*left2middle).T/=1000;
        std::cout<<"(*left2middle).T:"<<(*left2middle).T<<std::endl<<std::flush;
    }
    ui->statusBar->showMessage(tr("calibrate two done"), 2000);
}

void MainWindow::on_action_Image_triggered()
{
    ui->action_Cloud->setChecked(false);
    ui->action_Depth->setChecked(false);
    saveMode=ZedCameraThread::SaveMode::IMAGE;
}

void MainWindow::on_action_Cloud_triggered()
{
    ui->action_Image->setChecked(false);
    ui->action_Depth->setChecked(false);
    saveMode=ZedCameraThread::SaveMode::CLOUD;
}

void MainWindow::on_action_Depth_triggered()
{
    ui->action_Cloud->setChecked(false);
    ui->action_Image->setChecked(false);
    saveMode=ZedCameraThread::SaveMode::DEPTH;
}

void MainWindow::on_mergeCloud_triggered()
{
    if(middleMesh.getPoints().size()>0){
        Eigen::Matrix3Xf origin=middleMesh.getPoints();
        Eigen::Matrix3Xf points=origin;
        Eigen::Matrix4Xf colors=middleMesh.getColors();
        if(leftMesh.getPoints().size()>0&&left2middle){
            Eigen::Matrix3Xf tmp=leftMesh.getPoints();
            Eigen::Matrix4Xf tmpC=leftMesh.getColors();
            applyRigidTransform(tmp,*left2middle);
            double error;
            Rigid::PointCloudRegister<float>::registerPoints(tmp,origin,error,3);

            Eigen::Matrix3Xf P(points.rows(), points.cols()+tmp.cols());
            P<<points,tmp;
            points=P;

            Eigen::Matrix4Xf C(colors.rows(), colors.cols()+tmpC.cols());
            C<<colors,tmpC;
            colors=C;
            std::cout<<"merge left and middle done!"<<std::endl<<std::flush;
            std::cout<<"result.rows:"<<points.rows()<<",result.cols:"<<points.cols()<<std::endl<<std::flush;
        }
        if(rightMesh.getPoints().size()>0&&right2middle){
            Eigen::Matrix3Xf tmp=rightMesh.getPoints();
            Eigen::Matrix4Xf tmpC=rightMesh.getColors();
            applyRigidTransform(tmp,*right2middle);
            double error;
            Rigid::PointCloudRegister<float>::registerPoints(tmp,origin,error,3);

            Eigen::Matrix3Xf P(points.rows(), points.cols()+tmp.cols());
            P<<points,tmp;
            points=P;

            Eigen::Matrix4Xf C(colors.rows(), colors.cols()+tmpC.cols());
            C<<colors,tmpC;
            colors=C;
            std::cout<<"merge right and middle done!"<<std::endl<<std::flush;
            std::cout<<"result.rows:"<<points.rows()<<",result.cols:"<<points.cols()<<std::endl<<std::flush;
        }
        EigenMesh src;
        src.setPoints(points);
        src.setColors(colors);
        ModelWindow* modelWindow=new ModelWindow(this);
        modelWindow->inputSrcMesh(src);
        modelWindow->show();
    }else{

    }

}

void MainWindow::on_right2middle_clicked()
{
    ui->statusBar->showMessage(tr("calibrate two..."), 2000);
    detect=false;
    Calibrator srcCalibrator,dstCalibrator;
    StereoCalibrator stereoCalibrator;
    QString outputDir=sets.getOutputDir();
    QDir tmp;
    tmp.mkpath(outputDir);
    QString rightOutputFileName=outputDir+QDir::separator()+"right.yml";
    QString middleOutputFileName=outputDir+QDir::separator()+"middle.yml";
    QString relativeOutputFileName=outputDir+QDir::separator()+"right2middle.yml";
    if(rightGrayMats.size()>0&&middleGrayMats.size()>0){
        srcCalibrator.setOutputFileName(rightOutputFileName.toStdString());
        srcCalibrator.setImages(rightMats);
        srcCalibrator.setGrayImages(rightGrayMats);
        srcCalibrator.setChessBoardConfig(sets.chessBoardConfig);

        dstCalibrator.setOutputFileName(middleOutputFileName.toStdString());
        dstCalibrator.setImages(middleMats);
        dstCalibrator.setGrayImages(middleGrayMats);
        dstCalibrator.setChessBoardConfig(sets.chessBoardConfig);


    }else{
        QString imageDir=sets.getImageDir();
        QString imageSuffix=sets.getImageSurffix();
        QDir dir(imageDir);
        if(!dir.exists())return;
        QString rightImageDir=imageDir+QDir::separator()+"right";
        QDir rightDir(rightImageDir);
        if(!rightDir.exists())return;
        QString middleImageDir=imageDir+QDir::separator()+"middle";
        QDir middleDir(middleImageDir);
        if(!middleDir.exists())return;

        rightDir.setFilter(QDir::Files);
        QFileInfoList rightFileList = rightDir.entryInfoList();
        int rightFileCount = rightFileList.count();

        middleDir.setFilter(QDir::Files);
        QFileInfoList middleFileList = middleDir.entryInfoList();
        int middleFileCount = middleFileList.count();
        if(middleFileCount==0||middleFileCount==0||rightFileCount!=middleFileCount)return;
        std::vector<cv::Mat> rightImgs;
        std::vector<cv::Mat> rightGrayImgs;

        std::vector<cv::Mat> middleImgs;
        std::vector<cv::Mat> middleGrayImgs;
        for(int i=0;i<rightFileCount;i++)
        {
            QFileInfo rightFileInfo = rightFileList[i];
            QString rightSuffix = rightFileInfo.suffix();
            if(QString::compare(rightSuffix, imageSuffix, Qt::CaseInsensitive) == 0) {
                QString filePath = rightFileInfo.absoluteFilePath();
                QString fileName = rightFileInfo.baseName();
                cv::Mat img=cv::imread(filePath.toStdString());
                rightImgs.push_back(img);
                cv::Mat gray;
                cvtColor(img,gray,CV_BGR2GRAY);
                rightGrayImgs.push_back(gray);
            }

            QFileInfo middleFileInfo = middleFileList[i];
            QString middleSuffix = middleFileInfo.suffix();
            if(QString::compare(middleSuffix, imageSuffix, Qt::CaseInsensitive) == 0) {
                QString filePath = middleFileInfo.absoluteFilePath();
                QString fileName = middleFileInfo.baseName();
                cv::Mat img=cv::imread(filePath.toStdString());
                middleImgs.push_back(img);
                cv::Mat gray;
                cvtColor(img,gray,CV_BGR2GRAY);
                middleGrayImgs.push_back(gray);
            }
        }
        if(rightImgs.size()!=middleImgs.size())return;
        srcCalibrator.setOutputFileName(rightOutputFileName.toStdString());
        srcCalibrator.setImages(rightImgs);
        srcCalibrator.setGrayImages(rightGrayImgs);
        srcCalibrator.setChessBoardConfig(sets.chessBoardConfig);

        dstCalibrator.setOutputFileName(middleOutputFileName.toStdString());
        dstCalibrator.setImages(middleImgs);
        dstCalibrator.setGrayImages(middleGrayImgs);
        dstCalibrator.setChessBoardConfig(sets.chessBoardConfig);
    }
    if(useSDKParam){
        convertSDKParam(rightZed->getCameraInformation().calibration_parameters.right_cam,srcCalibrator);
        srcCalibrator.setUseSDKParam(true);
        convertSDKParam(middleZed->getCameraInformation().calibration_parameters.right_cam,dstCalibrator);
        dstCalibrator.setUseSDKParam(true);
        stereoCalibrator.setUseSDKParam(true);
    }
    stereoCalibrator.setSrcCalibrator(srcCalibrator);
    stereoCalibrator.setDstCalibrator(dstCalibrator);
    stereoCalibrator.setRelativeOutputFileName(relativeOutputFileName.toStdString());
    stereoCalibrator.calibrateStereo();
    right2middle=new CalibrateResult();
    *right2middle=stereoCalibrator.getRelative();
    if(initParams.coordinate_units == UNIT_METER){
        (*right2middle).T/=1000;
        std::cout<<"(*right2middle).T:"<<(*right2middle).T<<std::endl<<std::flush;
    }
    ui->statusBar->showMessage(tr("calibrate two done"), 2000);
}

void MainWindow::showLeft()
{
    QPixmap leftPix;
    if(leftShowQueue->TryPop(leftPix)){
        //leftScene->clear();
        leftScene->addPixmap(leftPix);
        //std::cout<<"showLeft"<<std::endl<<std::flush;
    }
}

void MainWindow::showMiddle()
{
    QPixmap middlePix;
    //middleShowQueue->WaitAndPop(middlePix);
    if(middleShowQueue->TryPop(middlePix)){
        //middleScene->clear();
        middleScene->addPixmap(middlePix);
        //std::cout<<"showMiddle"<<std::endl<<std::flush;
    }
}

void MainWindow::showRight()
{
    QPixmap rightPix;
    //rightShowQueue->WaitAndPop(rightPix);
    if(rightShowQueue->TryPop(rightPix)){
        //rightScene->clear();
        rightScene->addPixmap(rightPix);
        //std::cout<<"showRight"<<std::endl<<std::flush;
    }
}

