#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QPixmap>
#include <QDir>
#include <QCameraInfo>
#include "dialog.h"

#include "qfilefunctions.h"
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
    useSDKParam;
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
        std::cout << leftZed->getCameraInformation().camera_model << " n*" << leftId << " -> Result: " << sl::toString(err) << endl;
        delete leftZed;
        exit(EXIT_FAILURE);
    } else
        std::cout << leftZed->getCameraInformation().camera_model << " n*" << leftId << " SN " <<
                     leftZed->getCameraInformation().serial_number << " -> Result: " << sl::toString(err) << endl;
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

void MainWindow::saveLeft(QString imageDir,QString drawDir)
{
    Settings &sets=Settings::instance();
    QString surffix=sets.getImageSurffix();
    QDir tmp;
    QString leftImageDir=imageDir+QDir::separator()+"left";
    tmp.mkpath(leftImageDir);
    QString leftDrawImageDir=drawDir+QDir::separator()+"left";
    tmp.mkpath(leftDrawImageDir);
    leftNum++;
    QString leftPath=leftImageDir+QDir::separator()+"left"+QString::number(leftNum)+"."+surffix;
    if(!left.empty())cv::imwrite(leftPath.toStdString(),left);
    if(!leftDrawMat.empty()){
        leftPath=leftDrawImageDir+QDir::separator()+"left"+QString::number(leftNum)+"."+surffix;
        cv::imwrite(leftPath.toStdString(),leftDrawMat);
    }
    leftMats.push_back(left);
    leftGrayMats.push_back(leftGrayMat);
    ui->leftLcdNumber->display(leftNum);
}

void MainWindow::saveMiddle(QString imageDir, QString drawDir)
{
    Settings &sets=Settings::instance();
    QString surffix=sets.getImageSurffix();
    QDir tmp;
    QString middleImageDir=imageDir+QDir::separator()+"middle";
    tmp.mkpath(middleImageDir);
    QString middleDrawImageDir=drawDir+QDir::separator()+"middle";
    tmp.mkpath(middleDrawImageDir);
    middleNum++;
    QString middlePath=middleImageDir+QDir::separator()+"middle"+QString::number(middleNum)+"."+surffix;
    if(!middle.empty())cv::imwrite(middlePath.toStdString(),middle);
    if(!leftDrawMat.empty()){
        middlePath=middleDrawImageDir+QDir::separator()+"middle"+QString::number(middleNum)+"."+surffix;
        cv::imwrite(middlePath.toStdString(),middleDrawMat);
    }
    middleMats.push_back(middle);
    middleGrayMats.push_back(middleGrayMat);
    ui->middleLcdNumber->display(middleNum);
}

void MainWindow::saveRight(QString imageDir,QString drawDir)
{
    Settings &sets=Settings::instance();
    QString surffix=sets.getImageSurffix();
    QDir tmp;
    QString rightImageDir=imageDir+QDir::separator()+"right";
    tmp.mkpath(rightImageDir);
    QString rightDrawImageDir=drawDir+QDir::separator()+"right";
    tmp.mkpath(rightDrawImageDir);

    rightNum++;
    QString rightPath=rightImageDir+QDir::separator()+"right"+QString::number(rightNum)+"."+surffix;
    if(!right.empty())cv::imwrite(rightPath.toStdString(),right);
    if(!rightDrawMat.empty()){
        rightPath=rightDrawImageDir+QDir::separator()+"right"+QString::number(rightNum)+"."+surffix;
        cv::imwrite(rightPath.toStdString(),rightDrawMat);
    }
    rightMats.push_back(right);
    rightGrayMats.push_back(rightGrayMat);
    ui->rightLcdNumber->display(rightNum);
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
    //    leftCapture>>leftFrame;
    //    left=leftFrame(leftRect);
    int width, height;
    sl::Mat aux;
    sl::RuntimeParameters rt_params;
    if(!leftStopSignal){
        width=leftZed->getResolution().width;
        height=leftZed->getResolution().height;
        sl::ERROR_CODE res = leftZed->grab(rt_params);
        if (res == sl::SUCCESS) {
            leftZed->retrieveImage(aux, sl::VIEW_LEFT, sl::MEM_CPU);
            //cv::Mat(aux.getHeight(), aux.getWidth(), CV_8UC4, aux.getPtr<sl::uchar1>(sl::MEM_CPU)).copyTo(leftSbSResult(cv::Rect(0, 0, width, height)));
            //            leftZed->retrieveImage(aux, sl::VIEW_DEPTH, sl::MEM_CPU);
            //            cv::Mat(aux.getHeight(), aux.getWidth(), CV_8UC4, aux.getPtr<sl::uchar1>(sl::MEM_CPU)).copyTo(leftSbSResult(cv::Rect(width, 0, width, height)));
            //left=leftSbSResult(cv::Rect(0, 0, width, height));
            cv::Mat(aux.getHeight(), aux.getWidth(), CV_8UC4, aux.getPtr<sl::uchar1>(sl::MEM_CPU)).copyTo(left);
            cv::Mat leftTmp;
            cv::cvtColor(left,leftTmp,CV_BGR2RGB);
            if(detect){
                cv::cvtColor(left,leftGrayMat,CV_BGR2GRAY);
                std::vector<cv::Point2f> leftPointBuf;
                cv::Size checkerBoardSize=cv::Size(sets.chessBoardConfig.boardW,sets.chessBoardConfig.boardH);
                //extract image corner
                int leftFound=cv::findChessboardCorners(left,checkerBoardSize,leftPointBuf, CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS);//the  input arguments  is important
                if(leftFound){
                    //qDebug()<<"check left corners successful!";
                    leftDrawMat=left.clone();
                    cv::cornerSubPix( leftGrayMat, leftPointBuf, cv::Size(11,11),
                                      cv::Size(-1,-1), cv::TermCriteria( CV_TERMCRIT_EPS+CV_TERMCRIT_ITER, 30, 0.1 ));
                    cv::drawChessboardCorners(leftDrawMat,checkerBoardSize,leftPointBuf,leftFound);
                    cv::cvtColor(leftDrawMat,leftTmp,CV_BGR2RGB);

                }else{
                    //qDebug()<<"check left corners fail!";
                }
            }
            QImage leftImage=QImage((const uchar*)(leftTmp.data),leftTmp.cols,leftTmp.rows,QImage::Format_RGB888);
            QPixmap leftPix=QPixmap::fromImage(leftImage);
            leftScene->clear();
            leftScene->addPixmap(leftPix);
        }
        sl::sleep_ms(1);
    }
}

void MainWindow::readMiddleCamera()
{
    if(capturing)return;
    int width, height;
    sl::Mat aux;
    sl::RuntimeParameters rt_params;
    if(!middleStopSignal){
        width=middleZed->getResolution().width;
        height=middleZed->getResolution().height;
        sl::ERROR_CODE res = middleZed->grab(rt_params);
        if (res == sl::SUCCESS) {
            middleZed->retrieveImage(aux, sl::VIEW_LEFT, sl::MEM_CPU);
            //cv::Mat(aux.getHeight(), aux.getWidth(), CV_8UC4, aux.getPtr<sl::uchar1>(sl::MEM_CPU)).copyTo(middleSbSResult(cv::Rect(0, 0, width, height)));
            //            leftZed->retrieveImage(aux, sl::VIEW_DEPTH, sl::MEM_CPU);
            //            cv::Mat(aux.getHeight(), aux.getWidth(), CV_8UC4, aux.getPtr<sl::uchar1>(sl::MEM_CPU)).copyTo(leftSbSResult(cv::Rect(width, 0, width, height)));
            //middle=middleSbSResult(cv::Rect(0, 0, width, height));
            cv::Mat(aux.getHeight(), aux.getWidth(), CV_8UC4, aux.getPtr<sl::uchar1>(sl::MEM_CPU)).copyTo(middle);
            cv::Mat middleTmp;
            cv::cvtColor(middle,middleTmp,CV_BGR2RGB);
            if(detect){
                cv::cvtColor(middle,middleGrayMat,CV_BGR2GRAY);
                std::vector<cv::Point2f> middlePointBuf;
                cv::Size checkerBoardSize=cv::Size(sets.chessBoardConfig.boardW,sets.chessBoardConfig.boardH);
                //extract image corner
                int middleFound=cv::findChessboardCorners(middle,checkerBoardSize,middlePointBuf, CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS);//the  input arguments  is important
                if(middleFound){
                    //qDebug()<<"check middle corners successful!";
                    middleDrawMat=middle.clone();
                    cv::cornerSubPix( middleGrayMat, middlePointBuf, cv::Size(11,11),
                                      cv::Size(-1,-1), cv::TermCriteria( CV_TERMCRIT_EPS+CV_TERMCRIT_ITER, 30, 0.1 ));
                    cv::drawChessboardCorners(middleDrawMat,checkerBoardSize,middlePointBuf,middleFound);
                    cv::cvtColor(middleDrawMat,middleTmp,CV_BGR2RGB);

                }else{
                    //qDebug()<<"check middle corners fail!";
                }
            }
            QImage middleImage=QImage((const uchar*)(middleTmp.data),middleTmp.cols,middleTmp.rows,QImage::Format_RGB888);
            QPixmap middlePix=QPixmap::fromImage(middleImage);
            middleScene->clear();
            middleScene->addPixmap(middlePix);
        }
        sl::sleep_ms(1);
    }
}

void MainWindow::readRightCamera()
{
    if(capturing)return;
    int width, height;
    sl::Mat aux;
    sl::RuntimeParameters rt_params;
    if(!rightStopSignal){
        width=rightZed->getResolution().width;
        height=rightZed->getResolution().height;
        sl::ERROR_CODE res =rightZed->grab(rt_params);
        if (res == sl::SUCCESS) {
            //rightZed->retrieveImage(aux, sl::VIEW_LEFT, sl::MEM_CPU);
            //cv::Mat(aux.getHeight(), aux.getWidth(), CV_8UC4, aux.getPtr<sl::uchar1>(sl::MEM_CPU)).copyTo(rightSbSResult(cv::Rect(0, 0, width, height)));
            //            leftZed->retrieveImage(aux, sl::VIEW_DEPTH, sl::MEM_CPU);
            //            cv::Mat(aux.getHeight(), aux.getWidth(), CV_8UC4, aux.getPtr<sl::uchar1>(sl::MEM_CPU)).copyTo(leftSbSResult(cv::Rect(width, 0, width, height)));
            //right=rightSbSResult(cv::Rect(0, 0, width, height));
            cv::Mat(aux.getHeight(), aux.getWidth(), CV_8UC4, aux.getPtr<sl::uchar1>(sl::MEM_CPU)).copyTo(right);
            cv::Mat rightTmp;
            cv::cvtColor(right,rightTmp,CV_BGR2RGB);
            if(detect){
                cv::cvtColor(right,rightGrayMat,CV_BGR2GRAY);
                std::vector<cv::Point2f> rightPointBuf;
                cv::Size checkerBoardSize=cv::Size(sets.chessBoardConfig.boardW,sets.chessBoardConfig.boardH);
                //extract image corner
                int rightFound=cv::findChessboardCorners(right,checkerBoardSize,rightPointBuf, CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS);//the  input arguments  is important
                if(rightFound){
                    qDebug()<<"check right corners successful!";
                    rightDrawMat=right.clone();
                    cv::cornerSubPix( rightGrayMat, rightPointBuf, cv::Size(11,11),
                                      cv::Size(-1,-1), cv::TermCriteria( CV_TERMCRIT_EPS+CV_TERMCRIT_ITER, 30, 0.1 ));
                    cv::drawChessboardCorners(rightDrawMat,checkerBoardSize,rightPointBuf,rightFound);
                    cv::cvtColor(rightDrawMat,rightTmp,CV_BGR2RGB);

                }else{
                    qDebug()<<"check right corners fail!";
                }
            }
            QImage rightImage=QImage((const uchar*)(rightTmp.data),rightTmp.cols,rightTmp.rows,QImage::Format_RGB888);
            QPixmap rightPix=QPixmap::fromImage(rightImage);
            rightScene->clear();
            rightScene->addPixmap(rightPix);
        }
        sl::sleep_ms(1);
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
    QDir tmp;
    tmp.mkpath(imageDir);
    tmp.mkpath(drawDir);

    saveLeft(imageDir,drawDir);
    saveMiddle(imageDir,drawDir);
    saveRight(imageDir,drawDir);

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
    QDir tmp;
    tmp.mkpath(imageDir);
    tmp.mkpath(drawDir);
    saveLeft(imageDir,drawDir);
}

void MainWindow::on_saveRight_clicked()
{
    QString imageDir=sets.getImageDir();
    QString drawDir=sets.getDrawDir();
    QDir tmp;
    tmp.mkpath(imageDir);
    tmp.mkpath(drawDir);
    saveRight(imageDir,drawDir);
}

void MainWindow::on_saveMiddle_clicked()
{
    QString imageDir=sets.getImageDir();
    QString drawDir=sets.getDrawDir();
    QDir tmp;
    tmp.mkpath(imageDir);
    tmp.mkpath(drawDir);
    saveMiddle(imageDir,drawDir);
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
