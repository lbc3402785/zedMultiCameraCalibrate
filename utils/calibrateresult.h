#ifndef CALIBRATERESULT_H
#define CALIBRATERESULT_H
#include <vector>
#include <opencv2/opencv.hpp>
class CalibrateResult
{
public:
    CalibrateResult();
    cv::Mat intrinsicMatrix;
    cv::Mat distortionCoeff;
    std::vector<cv::Mat> rvecs;
    std::vector<cv::Mat> tvecs;
    cv::Mat R;
    cv::Mat T;
    double repError;
    double aspectRatio;
    double rms;
    cv::Size imageSize;
    bool saveCameraParams(std::string filename,int flags);
    bool saveRelativeParams(std::string filename,int flags);
    bool loadRelativeCameraParams(std::string filename);
};

#endif // CALIBRATERESULT_H
