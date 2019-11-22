#include "calibrator.h"
#include <iostream>
Calibrator::Calibrator()
{
    calibrationFlags = 0;
    useSDKParam=false;
}

void Calibrator::calibrate()
{
    findChessBoardCorners();
    if(!useSDKParam)calibrateOneCamera();
    result.saveCameraParams(outputFileName,calibrationFlags);
}

std::vector<cv::Mat> Calibrator::getImages() const
{
    return images;
}

void Calibrator::setImages(const std::vector<cv::Mat> &value)
{
    images = value;
}

ChessBoardConfig Calibrator::getChessBoardConfig() const
{
    return chessBoardConfig;
}

void Calibrator::setChessBoardConfig(const ChessBoardConfig &value)
{
    chessBoardConfig = value;
}

std::vector<cv::Mat> Calibrator::getGrayImages() const
{
    return grayImages;
}

void Calibrator::setGrayImages(const std::vector<cv::Mat> &value)
{
    grayImages = value;
}

void Calibrator::findChessBoardCorners()
{
    size_t imageNum=images.size();
    std::cout<<"imageNum:"<<imageNum<<std::endl;
    successNum=0;
    failNum=0;
    std::vector<cv::Point2f> pointBuf;
    cv::Size checkerBoardSize=cv::Size(chessBoardConfig.boardW,chessBoardConfig.boardH);
    int boardN=chessBoardConfig.boardW*chessBoardConfig.boardH;
    imagePoints.clear();
    fullImagePoints.clear();
    objectPoints.clear();
    fullObjectPoints.clear();
    used.clear();
    fullImagePoints.resize(imageNum);
    fullObjectPoints.resize(imageNum);
    used.resize(imageNum);
    for(size_t i=0;i<imageNum;i++){
        cv::Mat camImg=images[i];
        cv::Mat grayCamImg=grayImages[i];
        int camFound=cv::findChessboardCorners(camImg,checkerBoardSize,pointBuf, CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS);//the  input arguments  is important
        if(camFound){
            cv::cornerSubPix( grayCamImg, pointBuf, cv::Size(11,11),
                              cv::Size(-1,-1), cv::TermCriteria( CV_TERMCRIT_EPS+CV_TERMCRIT_ITER, 30, 0.1 ));
            cv::drawChessboardCorners(camImg,checkerBoardSize,pointBuf,camFound);
            cv::namedWindow("window", CV_WINDOW_NORMAL);
            cv::imshow("window",camImg);
            cv::waitKey(500);
            std::vector<cv::Point3f> objPointVec;
            cv::Point3f    objPoint;
            for (int  j = 0; j < boardN;  ++j) {
                objPoint.x=(j/(chessBoardConfig.boardW))*chessBoardConfig.dx;
                objPoint.y=(j%(chessBoardConfig.boardW))*chessBoardConfig.dy;
                objPoint.z=0.0f;
                objPointVec.push_back(objPoint);
            }
            imagePoints.push_back(pointBuf);
            objectPoints.push_back(objPointVec);
            fullImagePoints[i]=(pointBuf);
            fullObjectPoints[i]=(objPointVec);
            successNum++;
            used[i]=true;
            std::cout<<"successNumber="<<successNum<<"\t\t\t\tcurIdx="<<i<<std::endl;
        }else{
            failNum++;
            used[i]=false;
            std::cout<<"Fail!!\t\t"<<failNum<<"\t\tcurIdx="<<i<<std::endl;
        }

    }
}

void Calibrator::calibrateOneCamera()
{
    std::cout<<"\n\n\ncalibrating....\npls wait a moment!\n...\n\n...";
    if (successNum<2)
        exit(EXIT_FAILURE);
    cv::Size imgSize=images[0].size();
    result.imageSize=imgSize;
    result.repError=cv::calibrateCamera(objectPoints,
                                        imagePoints,
                                        imgSize,
                                        result.instrisincMatrix,
                                        result.distortionCoeff,
                                        result.rvecs,
                                        result.tvecs,0|CV_CALIB_FIX_K4|CV_CALIB_FIX_K5);
}

bool Calibrator::getUseSDKParam() const
{
    return useSDKParam;
}

void Calibrator::setUseSDKParam(bool value)
{
    useSDKParam = value;
}

std::vector<std::vector<cv::Point2f> > Calibrator::getFullImagePoints() const
{
    return fullImagePoints;
}

void Calibrator::setFullImagePoints(const std::vector<std::vector<cv::Point2f> > &value)
{
    fullImagePoints = value;
}

std::vector<std::vector<cv::Point3f> > Calibrator::getFullObjectPoints() const
{
    return fullObjectPoints;
}

void Calibrator::setFullObjectPoints(const std::vector<std::vector<cv::Point3f> > &value)
{
    fullObjectPoints = value;
}

std::string Calibrator::getOutputFileName() const
{
    return outputFileName;
}

void Calibrator::setOutputFileName(const std::string &value)
{
    outputFileName = value;
}

std::vector<bool> Calibrator::getUsed() const
{
    return used;
}

void Calibrator::setUsed(const std::vector<bool> &value)
{
    used = value;
}



CalibrateResult Calibrator::getResult() const
{
    return result;
}

void Calibrator::setResult(const CalibrateResult &value)
{
    result = value;
}



int Calibrator::getCalibrationFlags() const
{
    return calibrationFlags;
}

void Calibrator::setCalibrationFlags(int value)
{
    calibrationFlags = value;
}

