#ifndef POINTCLOUDFUNCTIONS_H
#define POINTCLOUDFUNCTIONS_H

#include <opencv2/opencv.hpp>
#include <sl_zed/Camera.hpp>
class PointCloudFunctions
{
public:
    PointCloudFunctions();
    static bool saveXYZRGBPointCloud(const cv::Mat& cloud, std::string filename);
    static bool saveXYZRGBPointCloud(std::vector<cv::Vec4f>& cloud, std::string filename);
    static bool loadXYZRGBPointCloud(std::vector<cv::Vec4f>& data,std::string filename);
    static void applyTransformXYZRGBPointCloud(const cv::Mat& cloud,const cv::Mat& R,const cv::Mat&T,cv::Mat& out);
    static void applyTransformXYZRGBPointCloud(std::vector<cv::Vec4f>& cloud,const cv::Mat& R,const cv::Mat&T,std::vector<cv::Vec4f> & out);
};

#endif // POINTCLOUDFUNCTIONS_H
