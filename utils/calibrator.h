#ifndef CALIBRATOR_H
#define CALIBRATOR_H
#include <opencv2/opencv.hpp>
#include <vector>
#include "chessboardconfig.h"
#include "calibrateresult.h"
class Calibrator
{
public:
    Calibrator();
    void calibrate();
    std::vector<cv::Mat> getImages() const;
    void setImages(const std::vector<cv::Mat> &value);
    ChessBoardConfig getChessBoardConfig() const;
    void setChessBoardConfig(const ChessBoardConfig &value);

    std::vector<cv::Mat> getGrayImages() const;
    void setGrayImages(const std::vector<cv::Mat> &value);

    std::string getOutputFileName() const;
    void setOutputFileName(const std::string &value);

    std::vector<bool> getUsed() const;
    void setUsed(const std::vector<bool> &value);

    CalibrateResult getResult() const;
    void setResult(const CalibrateResult &value);

    int getCalibrationFlags() const;
    void setCalibrationFlags(int value);

    bool getUseSDKParam() const;
    void setUseSDKParam(bool value);

    std::vector<std::vector<cv::Point2f> > getFullImagePoints() const;
    void setFullImagePoints(const std::vector<std::vector<cv::Point2f> > &value);

    std::vector<std::vector<cv::Point3f> > getFullObjectPoints() const;
    void setFullObjectPoints(const std::vector<std::vector<cv::Point3f> > &value);

private:
    CalibrateResult result;
    int successNum;
    int failNum;
    int calibrationFlags;
    ChessBoardConfig chessBoardConfig;
    std::vector<cv::Mat> images;
    std::vector<cv::Mat> grayImages;
    std::vector<std::vector<cv::Point2f>> fullImagePoints;
    std::vector<std::vector<cv::Point2f>> imagePoints;
    std::vector<std::vector<cv::Point3f>> fullObjectPoints;
    std::vector<std::vector<cv::Point3f>> objectPoints;
    std::vector<bool> used;
    void findChessBoardCorners();
    void calibrateOneCamera();
    std::string outputFileName;
    bool useSDKParam;
};

#endif // CALIBRATOR_H
