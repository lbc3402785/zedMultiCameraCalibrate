#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QPixmap>
#include <QDir>
#include <QCameraInfo>
#include "dialog.h"

#include "qfilefunctions.h"
#include "modelwindow.h"
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
    leftNum=0;
    middleNum=0;
    rightNum=0;
    leftTimer=new QTimer(this);
    middleTimer=new QTimer(this);
    rightTimer=new QTimer(this);
    connect(leftTimer,SIGNAL(timeout()),this,SLOT(readLeftCamera()));
    connect(middleTimer,SIGNAL(timeout()),this,SLOT(readMiddleCamera()));
    connect(rightTimer,SIGNAL(timeout()),this,SLOT(readRightCamera()));
    connect(ui->open,SIGNAL(clicked()),this,SLOT(openCamera()));
    connect(ui->close,SIGNAL(clicked()),this,SLOT(closeCamera()));

    initParams.camera_resolution = RESOLUTION_HD1080;
    initParams.depth_mode = DEPTH_MODE_ULTRA;
    initParams.coordinate_units = UNIT_METER;
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
    saveMode=SaveMode::IMAGE;
}

MainWindow::~MainWindow()
{
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
    zed->setDepthMaxRangeValue(1);
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
    CalibrateResult result;
    result.instrisincMatrix=(cv::Mat_<double>()<<param.fx,0,param.cx,
                             0,param.fy,param.cy,
                             0,0,1);
    result.distortionCoeff=cv::Mat::zeros(5,1,CV_64F);
    calibrator.setResult(result);
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
    int width, height;

    if (err != sl::SUCCESS) {
        std::cout << leftZed->getCameraInformation().camera_model << " n*" << leftId << " -> Result: " << sl::toString(err) << std::endl;
        delete leftZed;
        exit(EXIT_FAILURE);
    } else
        std::cout << leftZed->getCameraInformation().camera_model << " n*" << leftId << " SN " <<
                     leftZed->getCameraInformation().serial_number << " -> Result: " << sl::toString(err) << std::endl;
    zedSetting(leftZed);
    width = leftZed->getResolution().width;
    height = leftZed->getResolution().height;
    //    leftSbSResult= cv::Mat(height, width * 2, CV_8UC4, 1);
    left= cv::Mat(height, width, CV_8UC4, 1);
}

void MainWindow::openMiddleZed(int middleId)
{
    if(middleZed)return;
    middleZed=new sl::Camera();
    initParams.input.setFromCameraID(middleId);
    sl::ERROR_CODE err =middleZed->open(initParams);
    int width, height;

    if (err != sl::SUCCESS) {
        std::cout << middleZed->getCameraInformation().camera_model << " n*" << middleId << " -> Result: " << sl::toString(err) << endl;
        delete middleZed;
        exit(EXIT_FAILURE);
    } else
        std::cout << middleZed->getCameraInformation().camera_model << " n*" << middleId << " SN " <<
                     middleZed->getCameraInformation().serial_number << " -> Result: " << sl::toString(err) << std::endl;
    zedSetting(middleZed);
    width = middleZed->getResolution().width;
    height = middleZed->getResolution().height;
    //    middleSbSResult= cv::Mat(height, width * 2, CV_8UC4, 1);
    middle= cv::Mat(height, width, CV_8UC4, 1);
}

//void MainWindow::openMiddle(int middleId)
//{
//    middleCapture=cv::VideoCapture(middleId);
//    if(!middleCapture.isOpened()){
//        qDebug()<<"open camera id:"<<middleId<<" fail!";
//    }
//    middleCapture.set(cv::CAP_PROP_FRAME_WIDTH, FRAME_WIDTH);
//    middleCapture.set(cv::CAP_PROP_FRAME_HEIGHT, FRAME_HEIGHT);
//}

//void MainWindow::openRight(int rightId)
//{
//    rightCapture=cv::VideoCapture(rightId);
//    if(!rightCapture.isOpened()){
//        qDebug()<<"open camera id:"<<rightId<<" fail!";
//    }
//    rightCapture.set(cv::CAP_PROP_FRAME_WIDTH, FRAME_WIDTH);
//    rightCapture.set(cv::CAP_PROP_FRAME_HEIGHT, FRAME_HEIGHT);
//}
void MainWindow::openRightZed(int rightId)
{
    if(rightZed)return;
    rightZed=new sl::Camera();
    initParams.input.setFromCameraID(rightId);
    sl::ERROR_CODE err =rightZed->open(initParams);


    if (err != sl::SUCCESS) {
        std::cout << rightZed->getCameraInformation().camera_model << " n°" << rightZed << " -> Result: " << sl::toString(err) << endl;
        delete rightZed;
        exit(EXIT_FAILURE);
    } else
        std::cout << rightZed->getCameraInformation().camera_model << " n°" << rightZed << " SN " <<
                     rightZed->getCameraInformation().serial_number << " -> Result: " << sl::toString(err) << std::endl;

    int width = rightZed->getResolution().width;
    int height = rightZed->getResolution().height;
    //    rightSbSResult= cv::Mat(height, width * 2, CV_8UC4, 1);
    right= cv::Mat(height, width, CV_8UC4, 1);
}

void MainWindow::openAll(int leftId,int middleId,int rightId)
{
    //    openLeft(leftId);
    //    openMiddle(middleId);
    //    openRight(rightId);
    openLeftZed(leftId);
    openMiddleZed(middleId);
    openRightZed(rightId);
}

void MainWindow::saveLeft(QString imageDir,QString drawDir,QString outputDir)
{
    Settings &sets=Settings::instance();
    QString surffix=sets.getImageSurffix();
    QDir tmp;
    QString leftImageDir=imageDir+QDir::separator()+"left";
    tmp.mkpath(leftImageDir);
    QString leftDrawImageDir=drawDir+QDir::separator()+"left";
    tmp.mkpath(leftDrawImageDir);
    switch (saveMode) {
    case SaveMode::IMAGE:
        if(readZedImage(leftZed,left,true)){
            leftNum++;
            QString leftPath=leftImageDir+QDir::separator()+"left"+QString::number(leftNum)+"."+surffix;
            cv::Mat leftTmp;
            cv::cvtColor(left,leftTmp,CV_BGR2RGB);
            if(detect){
                if(detectAndDrawCorners(left,leftGrayMat,leftDrawMat)){
                    cv::cvtColor(leftDrawMat,leftTmp,CV_BGR2RGB);
                }
            }
            QImage leftImage=QImage((const uchar*)(leftTmp.data),leftTmp.cols,leftTmp.rows,QImage::Format_RGB888);
            QPixmap leftPix=QPixmap::fromImage(leftImage);
            leftScene->clear();
            leftScene->addPixmap(leftPix);
            //save image
            cv::imwrite(leftPath.toStdString(),left);
            leftPath=leftDrawImageDir+QDir::separator()+"left"+QString::number(leftNum)+"."+surffix;
            cv::imwrite(leftPath.toStdString(),leftDrawMat);

            leftMats.push_back(left);
            leftGrayMats.push_back(leftGrayMat);
            ui->leftLcdNumber->display(leftNum);
        }
        break;
    case SaveMode::DEPTH:
        if(readZedDepth(leftZed,leftDepth,true)){
            QString leftDepthPath=outputDir+QDir::separator()+"leftDepth.png";
            saveDepth(leftDepth,leftDepthPath.toStdString());
            std::cout<<"save depth "<<leftDepthPath.toStdString()<<" done!"<<std::endl;
        }
        break;
    case SaveMode::CLOUD:
        if(readZedCloud(leftZed,leftCloud,true)){
            QString leftCloudPath=outputDir+QDir::separator()+"left.obj";
            saveCloud(leftCloud,leftCloudPath.toStdString());
        }
        break;
    }
}

void MainWindow::saveMiddle(QString imageDir, QString drawDir,QString outputDir)
{
    Settings &sets=Settings::instance();
    QString surffix=sets.getImageSurffix();
    QDir tmp;
    QString middleImageDir=imageDir+QDir::separator()+"middle";
    tmp.mkpath(middleImageDir);
    QString middleDrawImageDir=drawDir+QDir::separator()+"middle";
    tmp.mkpath(middleDrawImageDir);
    switch (saveMode) {
    case SaveMode::IMAGE:
        if(readZedImage(middleZed,middle,true)){
            middleNum++;
            QString middlePath=middleImageDir+QDir::separator()+"middle"+QString::number(middleNum)+"."+surffix;
            cv::Mat middleTmp;
            cv::cvtColor(middle,middleTmp,CV_BGR2RGB);
            if(detect){
                if(detectAndDrawCorners(middle,middleGrayMat,middleDrawMat)){
                    cv::cvtColor(middleDrawMat,middleTmp,CV_BGR2RGB);
                }
            }
            QImage middleImage=QImage((const uchar*)(middleTmp.data),middleTmp.cols,middleTmp.rows,QImage::Format_RGB888);
            QPixmap middlePix=QPixmap::fromImage(middleImage);
            middleScene->clear();
            middleScene->addPixmap(middlePix);
            //save image
            cv::imwrite(middlePath.toStdString(),middle);
            middlePath=middleDrawImageDir+QDir::separator()+"middle"+QString::number(middleNum)+"."+surffix;
            cv::imwrite(middlePath.toStdString(),middleDrawMat);

            middleMats.push_back(middle);
            middleGrayMats.push_back(middleGrayMat);
            ui->middleLcdNumber->display(middleNum);
        }
        break;
    case SaveMode::DEPTH:
        if(readZedDepth(middleZed,middleDepth,true)){
            QString middleDepthPath=outputDir+QDir::separator()+"middleDepth.png";
            saveDepth(middleDepth,middleDepthPath.toStdString());
            std::cout<<"save depth "<<middleDepthPath.toStdString()<<" done!"<<std::endl;
        }
        break;
    case SaveMode::CLOUD:
        if(readZedCloud(middleZed,middleCloud,true)){
            QString middleCloudPath=outputDir+QDir::separator()+"middle.obj";
            saveCloud(middleCloud,middleCloudPath.toStdString());
        }
        break;
    }
}

void MainWindow::saveRight(QString imageDir,QString drawDir,QString outputDir)
{
    Settings &sets=Settings::instance();
    QString surffix=sets.getImageSurffix();
    QDir tmp;
    QString rightImageDir=imageDir+QDir::separator()+"right";
    tmp.mkpath(rightImageDir);
    QString rightDrawImageDir=drawDir+QDir::separator()+"right";
    tmp.mkpath(rightDrawImageDir);
    switch (saveMode) {
    case SaveMode::IMAGE:
        if(readZedImage(rightZed,right,true)){
            rightNum++;
            QString rightPath=rightImageDir+QDir::separator()+"right"+QString::number(rightNum)+"."+surffix;
            cv::Mat rightTmp;
            cv::cvtColor(right,rightTmp,CV_BGR2RGB);
            if(detect){
                if(detectAndDrawCorners(right,rightGrayMat,rightDrawMat)){
                    cv::cvtColor(rightDrawMat,rightTmp,CV_BGR2RGB);
                }
            }
            QImage rightImage=QImage((const uchar*)(rightTmp.data),rightTmp.cols,rightTmp.rows,QImage::Format_RGB888);
            QPixmap rightPix=QPixmap::fromImage(rightImage);
            rightScene->clear();
            rightScene->addPixmap(rightPix);
            //save image
            cv::imwrite(rightPath.toStdString(),right);
            rightPath=rightDrawImageDir+QDir::separator()+"right"+QString::number(rightNum)+"."+surffix;
            cv::imwrite(rightPath.toStdString(),rightDrawMat);

            rightMats.push_back(right);
            rightGrayMats.push_back(rightGrayMat);
            ui->rightLcdNumber->display(rightNum);
        }
        break;
    case SaveMode::DEPTH:
        if(readZedDepth(rightZed,rightDepth,true)){
            QString rightDepthPath=outputDir+QDir::separator()+"rightDepth.png";
            saveDepth(rightDepth,rightDepthPath.toStdString());
            std::cout<<"save depth "<<rightDepthPath.toStdString()<<" done!"<<std::endl;
        }
        break;
    case SaveMode::CLOUD:
        if(readZedCloud(rightZed,rightCloud,true)){
            QString rightCloudPath=outputDir+QDir::separator()+"right.obj";
            saveDepth(rightDepth,rightCloudPath.toStdString());
        }
        break;
    }
}

bool MainWindow::readZedData(Camera *zed, cv::Mat &out,SaveMode mode, bool wait)
{
    if(zed==nullptr){
        std::cout<<"camera is nullptr"<<std::endl;
        return false;
    }
    sl::Mat aux;
    sl::RuntimeParameters rt_params;
    if(wait){
        while(true){
            sl::ERROR_CODE res = zed->grab(rt_params);
            if (res == sl::SUCCESS) {
                switch (mode) {
                case SaveMode::IMAGE:
                    zed->retrieveImage(aux, sl::VIEW_LEFT, sl::MEM_CPU);
                    out=cv::Mat(aux.getHeight(), aux.getWidth(), CV_8UC4, aux.getPtr<sl::uchar1>(sl::MEM_CPU)).clone();
                    break;
                case SaveMode::DEPTH:
                    zed->retrieveMeasure(aux, sl::MEASURE_DEPTH, sl::MEM_CPU);
                    out=cv::Mat(aux.getHeight(), aux.getWidth(), CV_32FC1, aux.getPtr<sl::float1>(sl::MEM_CPU)).clone();
                    break;
                case SaveMode::CLOUD:
                    zed->retrieveMeasure(aux, sl::MEASURE_XYZRGBA, sl::MEM_CPU);
                    out=cv::Mat(aux.getHeight(), aux.getWidth(), CV_32FC4, aux.getPtr<sl::float1>(sl::MEM_CPU)).clone();
                    break;
                }

                break;
            }else{
                sl::sleep_ms(1);
            }
        }
        return true;
    }else{

        sl::ERROR_CODE res = zed->grab(rt_params);
        if (res == sl::SUCCESS) {
            switch (mode) {
            case SaveMode::IMAGE:
                zed->retrieveImage(aux, sl::VIEW_LEFT, sl::MEM_CPU);
                cv::Mat(aux.getHeight(), aux.getWidth(), CV_8UC4, aux.getPtr<sl::uchar1>(sl::MEM_CPU)).copyTo(out);
                break;
            case SaveMode::DEPTH:
                zed->retrieveMeasure(aux, sl::MEASURE_DEPTH, sl::MEM_CPU);
                cv::Mat(aux.getHeight(), aux.getWidth(), CV_32FC1, aux.getPtr<sl::float1>(sl::MEM_CPU)).copyTo(out);
                break;
            case SaveMode::CLOUD:
                zed->retrieveMeasure(aux, sl::MEASURE_XYZRGBA, sl::MEM_CPU);
                cv::Mat(aux.getHeight(), aux.getWidth(), CV_32FC4, aux.getPtr<sl::float1>(sl::MEM_CPU)).copyTo(out);
                break;
            }
            return true;
        }else{
            return false;
        }
    }
}

bool MainWindow::readZedImage(Camera *zed, cv::Mat &image,bool wait)
{
    return readZedData(zed,image,SaveMode::IMAGE,wait);
}

bool MainWindow::readZedDepth(Camera *zed, cv::Mat &depth, bool wait)
{
    return readZedData(zed,depth,SaveMode::DEPTH,wait);
}

bool MainWindow::readZedCloud(Camera *zed, cv::Mat &cloud, bool wait)
{
    return readZedData(zed,cloud,SaveMode::CLOUD,wait);
}

bool MainWindow::detectAndDrawCorners(cv::Mat &image, cv::Mat &grayImage, cv::Mat &drawImage)
{
    cv::cvtColor(image,grayImage,CV_BGR2GRAY);
    std::vector<cv::Point2f> leftPointBuf;
    cv::Size checkerBoardSize=cv::Size(sets.chessBoardConfig.boardW,sets.chessBoardConfig.boardH);
    //extract image corner
    int leftFound=cv::findChessboardCorners(left,checkerBoardSize,leftPointBuf, CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS);//the  input arguments  is important
    if(leftFound){
        //qDebug()<<"check left corners successful!";
        drawImage=image.clone();
        cv::cornerSubPix( grayImage, leftPointBuf, cv::Size(11,11),
                          cv::Size(-1,-1), cv::TermCriteria( CV_TERMCRIT_EPS+CV_TERMCRIT_ITER, 30, 0.1 ));
        cv::drawChessboardCorners(drawImage,checkerBoardSize,leftPointBuf,leftFound);
        return true;
    }else{
        return false;
    }
}

void MainWindow::saveDepth(cv::Mat depth, std::string savePath)
{
    depth = cv::max(cv::min(depth*depthFactor, 65000.0), 0.0);
    imwrite(savePath, depth);
}

void MainWindow::saveCloud(cv::Mat cloud, std::string savePath)
{
    std::ofstream out(savePath);
    float zmin = 65536;
    float  zmax = -65535;
    int i,j;
    std::cout<<" cloud.rows:"<< cloud.rows<<" cloud.cols:"<< cloud.cols<<std::endl<<std::flush;
    for (i = 0; i < cloud.rows; i++) {
        for (j = 0; j < cloud.cols; j++) {
            cv::Vec4f f4=cloud.at<cv::Vec4f>(i,j);
            if (std::isnan(f4[0]) || std::isnan(f4[1]) || std::isnan(f4[2]))continue;
            if (std::isinf(f4[0]) || std::isinf(f4[1]) || std::isinf(f4[2]))continue;
            if (f4[2] > zmax)zmax = f4[2];
            if (f4[2] < zmin)zmin = f4[2];
            out << "v " << f4[0] << " " <<f4[1] << " " << f4[2] << " ";
            unsigned char* temp= (unsigned char*)&f4[3];
            out << (int)temp[0] << " " << (int)temp[1] << " " << (int)temp[2];
            out << std::endl;
        }
    }
    out.close();
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
    leftTimer->start(10);
    middleTimer->start(10);
    rightTimer->start(10);
    ui->leftId->setDisabled(true);
    ui->middleId->setDisabled(true);
    ui->rightId->setDisabled(true);
}

void MainWindow::readCamera()
{
    readLeftCamera();
    readMiddleCamera();
    readRightCamera();
}

void MainWindow::readLeftCamera()
{
    if(capturing)return;
    if(!leftStopSignal){
        if (readZedImage(leftZed,left)) {
            cv::Mat leftTmp;
            cv::cvtColor(left,leftTmp,CV_BGR2RGB);
            if(detect){
                if(detectAndDrawCorners(left,leftGrayMat,leftDrawMat)){
                    cv::cvtColor(leftDrawMat,leftTmp,CV_BGR2RGB);
                }
            }
            QImage leftImage=QImage((const uchar*)(leftTmp.data),leftTmp.cols,leftTmp.rows,QImage::Format_RGB888);
            QPixmap leftPix=QPixmap::fromImage(leftImage);
            leftScene->clear();
            leftScene->addPixmap(leftPix);
        }
    }
}

void MainWindow::readMiddleCamera()
{
    if(capturing)return;
    if(!middleStopSignal){
        if (readZedImage(middleZed,middle)) {
            cv::Mat middleTmp;
            cv::cvtColor(middle,middleTmp,CV_BGR2RGB);
            if(detect){
                if(detectAndDrawCorners(middle,middleGrayMat,middleDrawMat)){
                    cv::cvtColor(middleDrawMat,middleTmp,CV_BGR2RGB);
                }
            }
            QImage middleImage=QImage((const uchar*)(middleTmp.data),middleTmp.cols,middleTmp.rows,QImage::Format_RGB888);
            QPixmap middlePix=QPixmap::fromImage(middleImage);
            middleScene->clear();
            middleScene->addPixmap(middlePix);
        }
    }
}

void MainWindow::readRightCamera()
{
    if(capturing)return;
    if(!rightStopSignal){
        if (readZedImage(rightZed,right)) {
            cv::Mat rightTmp;
            cv::cvtColor(right,rightTmp,CV_BGR2RGB);
            if(detect){
                if(detectAndDrawCorners(right,rightGrayMat,rightDrawMat)){
                    cv::cvtColor(rightDrawMat,rightTmp,CV_BGR2RGB);
                }
            }
            QImage rightImage=QImage((const uchar*)(rightTmp.data),rightTmp.cols,rightTmp.rows,QImage::Format_RGB888);
            QPixmap rightPix=QPixmap::fromImage(rightImage);
            rightScene->clear();
            rightScene->addPixmap(rightPix);
        }
    }

}

void MainWindow::closeCamera()
{
    leftTimer->stop();
    middleTimer->stop();
    rightTimer->stop();
    ui->leftId->setDisabled(false);
    ui->middleId->setDisabled(false);
    ui->rightId->setDisabled(false);
    leftScene->clear();
    middleScene->clear();
    rightScene->clear();
}

void MainWindow::on_check_clicked()
{
    detect=!detect;
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
    leftTimer->start(10);
    ui->leftId->setDisabled(true);

}

void MainWindow::on_closeLeft_clicked()
{
    leftTimer->stop();
    ui->leftId->setDisabled(false);
    leftScene->clear();
}

void MainWindow::on_openRight_clicked()
{
    if(!checkCameraId()){
        ui->statusBar->showMessage(tr("select the same camera!"), 2000);
        return;
    }
    //openRight(ui->rightId->currentIndex());
    openRightZed(ui->rightId->currentIndex());
    rightTimer->start(10);
    ui->rightId->setDisabled(true);
}

void MainWindow::on_closeRight_clicked()
{
    rightTimer->stop();
    ui->rightId->setDisabled(false);
    rightScene->clear();
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
    middleTimer->start(10);
    ui->middleId->setDisabled(true);
}

void MainWindow::on_closeMiddle_clicked()
{
    middleTimer->stop();
    ui->middleId->setDisabled(false);
    middleScene->clear();
}


void MainWindow::on_clearFile_triggered()
{
    QFileFunctions::clearFolder(sets.getDrawDir(),false);
    QFileFunctions::clearFolder(sets.getImageDir(),false);
    QFileFunctions::clearFolder(sets.getOutputDir(),false);
    leftNum=0;
    middleNum=0;
    rightNum=0;
}

void MainWindow::on_left2middle_clicked()
{
    ui->statusBar->showMessage(tr("calibrate two..."), 2000);
    Calibrator srcCalibrator,dstCalibrator;
    StereoCalibrator stereoCalibrator;
    QString outputDir=sets.getOutputDir();
    QDir tmp;
    tmp.mkpath(outputDir);
    QString leftOutputFileName=outputDir+QDir::separator()+"left.yml";
    QString middleOutputFileName=outputDir+QDir::separator()+"middle.yml";
    QString relativeOutputFileName=outputDir+QDir::separator()+"left2middle.yml";
    if(leftGrayMats.size()>0&&middleGrayMats.size()>0){
        srcCalibrator.setOutputFileName(leftOutputFileName.toStdString());
        srcCalibrator.setImages(leftMats);
        srcCalibrator.setGrayImages(leftGrayMats);
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
            std::cout<<"leftSuffix:"<<leftSuffix.toStdString()<<std::endl;
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
        srcCalibrator.setOutputFileName(leftOutputFileName.toStdString());
        srcCalibrator.setImages(leftImgs);
        srcCalibrator.setGrayImages(leftGrayImgs);
        srcCalibrator.setChessBoardConfig(sets.chessBoardConfig);

        dstCalibrator.setOutputFileName(middleOutputFileName.toStdString());
        dstCalibrator.setImages(middleImgs);
        dstCalibrator.setGrayImages(middleGrayImgs);
        dstCalibrator.setChessBoardConfig(sets.chessBoardConfig);
    }
    if(useSDKParam){
        convertSDKParam(leftZed->getCameraInformation().calibration_parameters.left_cam,srcCalibrator);
        srcCalibrator.setUseSDKParam(true);
        convertSDKParam(middleZed->getCameraInformation().calibration_parameters.left_cam,dstCalibrator);
        dstCalibrator.setUseSDKParam(true);
    }
    stereoCalibrator.setSrcCalibrator(srcCalibrator);
    stereoCalibrator.setDstCalibrator(dstCalibrator);
    stereoCalibrator.setRelativeOutputFileName(relativeOutputFileName.toStdString());

    stereoCalibrator.calibrateStereo();
    ui->statusBar->showMessage(tr("calibrate two done"), 2000);
}

void MainWindow::on_action_Image_triggered()
{
    ui->action_Cloud->setChecked(false);
    ui->action_Depth->setChecked(false);
    saveMode=SaveMode::IMAGE;
}

void MainWindow::on_action_Cloud_triggered()
{
    ui->action_Image->setChecked(false);
    ui->action_Depth->setChecked(false);
    saveMode=SaveMode::CLOUD;
}

void MainWindow::on_action_Depth_triggered()
{
    ui->action_Cloud->setChecked(false);
    ui->action_Image->setChecked(false);
    saveMode=SaveMode::DEPTH;
}

void MainWindow::on_mergeCloud_triggered()
{
    ModelWindow* modelWindow=new ModelWindow(this);
    modelWindow->show();
}
