#include "zedcamerathread.h"
#include <QPixmap>
#include <QDir>
#include <QCameraInfo>
static int interval=300;
using namespace sl;
ZedCameraThread::ZedCameraThread(QObject *parent):QThread(parent)
{
    showQueue=nullptr;
    useSDKParam=true;
    useRectified=false;
    depthFactor=1000;
    capturing=false;
    stopSignal=false;
}

ZedCameraThread::~ZedCameraThread()
{
    if(timer){
        timer->stop();
        delete timer;
    }
}
/**
 * ————————————————
 * 版权声明：本文为CSDN博主「qq_2579440213」的原创文章，遵循 CC 4.0 BY-SA 版权协议，转载请附上原文出处链接及本声明。
 * 原文链接：https://blog.csdn.net/qq_33474442/article/details/84753403
 */
void ZedCameraThread::run()
{
    timer=new QTimer();
    timer->start(interval);
    connect(timer,&QTimer::timeout,this,&ZedCameraThread::readCamera);
    this->exec();//此句必须有，否则线程会立即结束

}

bool ZedCameraThread::getStopSignal() const
{
    return stopSignal;
}

void ZedCameraThread::setStopSignal(bool value)
{
    stopSignal = value;
}

void ZedCameraThread::close()
{
    std::cout<<id<<" thread close"<<std::endl<<std::flush;
    stopSignal=true;
    timer->stop();
}

bool ZedCameraThread::getDetect() const
{
    return detect;
}

void ZedCameraThread::setDetect(bool value)
{
    detect = value;
}

ZedCameraThread::SaveMode ZedCameraThread::getSaveMode() const
{
    return saveMode;
}

void ZedCameraThread::setSaveMode(const SaveMode &value)
{
    saveMode = value;
}

ThreadSafeQueue<QPixmap> *ZedCameraThread::getShowQueue() const
{
    return showQueue;
}

void ZedCameraThread::setShowQueue(ThreadSafeQueue<QPixmap> *value)
{
    showQueue = value;
}

void ZedCameraThread::saveZedData(QString imageDir,QString drawDir,QString outputDir)
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
        if(readZedImage(zed,image,true)){

            cv::Mat leftTmp;
            cv::cvtColor(image,leftTmp,CV_BGR2RGB);
            if(detect){
                if(detectAndDrawCorners(image,imageGrayMat,imageDrawMat)){

                    QString leftPath=leftImageDir+QDir::separator()+"left"+QString::number(num)+"."+surffix;
                    cv::cvtColor(imageDrawMat,leftTmp,CV_BGR2RGB);
                    mats.push_back(image.clone());
                    grayMats.push_back(imageGrayMat.clone());
                    num++;
                    //save image
                    cv::imwrite(leftPath.toStdString(),image);
                    leftPath=leftDrawImageDir+QDir::separator()+"left"+QString::number(num)+"."+surffix;
                    cv::imwrite(leftPath.toStdString(),imageDrawMat);
                }
            }
            QImage leftImage=QImage((const uchar*)(leftTmp.data),leftTmp.cols,leftTmp.rows,QImage::Format_RGB888);
            QPixmap leftPix=QPixmap::fromImage(leftImage);
            showQueue->WaitAndPush(leftPix);
            emit returnQPixmap();
        }
        break;
    case SaveMode::DEPTH:
        if(readZedDepth(zed,depth,true)){
            QString leftDepthPath=outputDir+QDir::separator()+"leftDepth.png";
            saveDepth(depth,leftDepthPath.toStdString());
            std::cout<<"save depth "<<leftDepthPath.toStdString()<<" done!"<<std::endl;
        }
        break;
    case SaveMode::CLOUD:
        cv::Mat leftCloud;
        if(readZedCloud(zed,leftCloud,true)){
            QString leftCloudPath=outputDir+QDir::separator()+"left.obj";
            saveCloud(leftCloud,mesh,leftCloudPath.toStdString());
        }
        break;
    }
}

int ZedCameraThread::getId() const
{
    return id;
}

void ZedCameraThread::setId(int value)
{
    id = value;
}

bool ZedCameraThread::readZedData(Camera *zed, cv::Mat &out,SaveMode mode, bool wait)
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
                    zed->retrieveImage(aux, useRectified?sl::VIEW_LEFT:sl::VIEW_LEFT_UNRECTIFIED, sl::MEM_CPU);
                    out=cv::Mat(aux.getHeight(), aux.getWidth(), CV_8UC4, aux.getPtr<sl::uchar1>(sl::MEM_CPU)).clone();
                    cv::cvtColor(out,out,cv::COLOR_BGRA2BGR);
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

bool ZedCameraThread::readZedImage(Camera *zed, cv::Mat &image,bool wait)
{
    return readZedData(zed,image,SaveMode::IMAGE,wait);
}

bool ZedCameraThread::readZedDepth(Camera *zed, cv::Mat &depth, bool wait)
{
    return readZedData(zed,depth,SaveMode::DEPTH,wait);
}

bool ZedCameraThread::readZedCloud(Camera *zed, cv::Mat &cloud, bool wait)
{
    return readZedData(zed,cloud,SaveMode::CLOUD,wait);
}

bool ZedCameraThread::detectAndDrawCorners(cv::Mat &image, cv::Mat &grayImage, cv::Mat &drawImage)
{
    cv::cvtColor(image,grayImage,CV_BGR2GRAY);
    std::vector<cv::Point2f> leftPointBuf;
    cv::Size checkerBoardSize=cv::Size(sets.chessBoardConfig.boardW,sets.chessBoardConfig.boardH);
    //extract image corner
    int leftFound=cv::findChessboardCorners(image,checkerBoardSize,leftPointBuf, CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS);//the  input arguments  is important
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

void ZedCameraThread::readCamera()
{
    if(capturing)return;
    if(!stopSignal&&showQueue){
        if (readZedImage(zed,image)) {
            cv::Mat leftTmp;
            cv::cvtColor(image,leftTmp,CV_BGR2RGB);
            if(detect){
                if(detectAndDrawCorners(image,imageGrayMat,imageDrawMat)){
                    cv::cvtColor(imageDrawMat,leftTmp,CV_BGR2RGB);
                }
            }
            QImage leftImage=QImage((const uchar*)(leftTmp.data),leftTmp.cols,leftTmp.rows,QImage::Format_RGB888);
            QPixmap leftPix=QPixmap::fromImage(leftImage);
            showQueue->WaitAndPush(leftPix);
            emit returnQPixmap();
            //std::cout<<"returnQPixmap"<<std::endl<<std::flush;
        }
    }
}
void ZedCameraThread::saveDepth(cv::Mat depth, std::string savePath)
{
    depth = cv::max(cv::min(depth*depthFactor, 65000.0), 0.0);
    imwrite(savePath, depth);
}

void ZedCameraThread::saveCloud(cv::Mat cloud,EigenMesh& mesh, std::string savePath)
{
    std::vector<Eigen::Vector3f> pv;
    std::vector<Eigen::Vector4f> cv;
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
            pv.push_back(Eigen::Vector3f(f4[0],f4[1],f4[2]));
            unsigned char* temp= (unsigned char*)&f4[3];
            out << (int)temp[0] << " " << (int)temp[1] << " " << (int)temp[2];
            cv.push_back(Eigen::Vector4f((int)temp[0],(int)temp[1],(int)temp[2],255));
            out << std::endl;
        }
    }
    out.close();
    Eigen::Matrix3Xf points;
    Eigen::Matrix4Xf colors;
    points.resize(3,pv.size());
    colors.resize(4,cv.size());
    for(i=0;i<pv.size();i++){
        points.col(i)=pv[i];
        colors.col(i)=cv[i];
    }
    mesh.setPoints(points);
    mesh.setColors(colors);
}
