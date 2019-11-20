#ifndef SDKFUNCTIONS_H
#define SDKFUNCTIONS_H

#include <limits>
#include <thread>

#include <opencv2/opencv.hpp>
#include <sl_zed/Camera.hpp>
class SDKFunctions
{
public:
    SDKFunctions();
    static cv::Mat slMat2cvMat(sl::Mat& input);

};

#endif // SDKFUNCTIONS_H
