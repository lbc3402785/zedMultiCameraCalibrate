#include "calibrateresult.h"
#include <opencv2/imgproc.hpp>
CalibrateResult::CalibrateResult()
{
    aspectRatio=1.0;
    intrinsicMatrix=cv::Mat::eye(3,3,CV_64F);
    distortionCoeff=cv::Mat::zeros(4,1,CV_64F);
}

bool CalibrateResult::saveCameraParams(std::string filename,int flags)
{
    using namespace cv;
    FileStorage fs(filename, FileStorage::WRITE);
    if(!fs.isOpened())
        return false;

    time_t tt;
    time(&tt);
    struct tm *t2 = localtime(&tt);
    char buf[1024];
    strftime(buf, sizeof(buf) - 1, "%c", t2);

    fs << "calibration_time" << buf;

    fs << "image_width" << imageSize.width;
    fs << "image_height" << imageSize.height;

    if(flags & CALIB_FIX_ASPECT_RATIO) fs << "aspectRatio" << aspectRatio;

    if(flags != 0) {
        sprintf(buf, "flags: %s%s%s%s",
                flags & CALIB_USE_INTRINSIC_GUESS ? "+use_intrinsic_guess" : "",
                flags & CALIB_FIX_ASPECT_RATIO ? "+fix_aspectRatio" : "",
                flags & CALIB_FIX_PRINCIPAL_POINT ? "+fix_principal_point" : "",
                flags & CALIB_ZERO_TANGENT_DIST ? "+zero_tangent_dist" : "");
    }

    fs << "flags" << flags;

    fs << "camera_matrix" << intrinsicMatrix;
    fs << "distortion_coefficients" << distortionCoeff;

    fs << "avg_reprojection_error" << repError;

    return true;
}
bool CalibrateResult::saveRelativeParams(std::string filename, int flags)
{
    using namespace cv;
    FileStorage fs(filename, FileStorage::WRITE);
    if(!fs.isOpened())
        return false;

    time_t tt;
    time(&tt);
    struct tm *t2 = localtime(&tt);
    char buf[1024];
    strftime(buf, sizeof(buf) - 1, "%c", t2);

    fs << "calibration_time" << buf;

    fs << "image_width" << imageSize.width;
    fs << "image_height" << imageSize.height;

    if(flags & CALIB_FIX_ASPECT_RATIO) fs << "aspectRatio" << aspectRatio;

    if(flags != 0) {
        sprintf(buf, "flags: %s%s%s%s",
                flags & CALIB_USE_INTRINSIC_GUESS ? "+use_intrinsic_guess" : "",
                flags & CALIB_FIX_ASPECT_RATIO ? "+fix_aspectRatio" : "",
                flags & CALIB_FIX_PRINCIPAL_POINT ? "+fix_principal_point" : "",
                flags & CALIB_ZERO_TANGENT_DIST ? "+zero_tangent_dist" : "");
    }

    fs << "flags" << flags;

    fs << "R" << R;
    fs << "T" << T;

    fs << "rms" << rms;

    return true;
}

bool CalibrateResult::loadRelativeCameraParams(std::string filename)
{
    using namespace cv;
    FileStorage fs(filename, FileStorage::READ);
    if(!fs.isOpened())
        return false;
    fs["R"]>>R;
    fs["T"]>>T;
    std::cout<< "R:" << R;
    std::cout<< "T:" << T;
    return true;
}
