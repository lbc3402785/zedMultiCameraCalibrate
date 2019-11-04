#include "stereocalibrator.h"

StereoCalibrator::StereoCalibrator()
{
    calibrationFlags=0;
}



void StereoCalibrator::calibrateStereo()
{
    srcCalibrator.calibrate();
    dstCalibrator.calibrate();
    calRelative();
}

Calibrator StereoCalibrator::getSrcCalibrator() const
{
    return srcCalibrator;
}

void StereoCalibrator::setSrcCalibrator(const Calibrator &value)
{
    srcCalibrator = value;
}

Calibrator StereoCalibrator::getDstCalibrator() const
{
    return dstCalibrator;
}

void StereoCalibrator::setDstCalibrator(const Calibrator &value)
{
    dstCalibrator = value;
}
cv::Mat eulerAnglesToRotationMatrix(cv::Vec3f &theta)
{
    // Calculate rotation about x axis
    cv::Mat R_x = (cv::Mat_<double>(3,3) <<
                   1,       0,              0,
               0,       cos(theta[0]),   -sin(theta[0]),
               0,       sin(theta[0]),   cos(theta[0])
               );

    // Calculate rotation about y axis
    cv::Mat R_y = (cv::Mat_<double>(3,3) <<
               cos(theta[1]),    0,      sin(theta[1]),
               0,               1,      0,
               -sin(theta[1]),   0,      cos(theta[1])
               );

    // Calculate rotation about z axis
    cv::Mat R_z = (cv::Mat_<double>(3,3) <<
               cos(theta[2]),    -sin(theta[2]),      0,
               sin(theta[2]),    cos(theta[2]),       0,
               0,               0,                  1);


    // Combined rotation matrix
    cv::Mat R = R_z * R_y * R_x;

    return R;

}
void calsrc2dstRt(cv::Mat srcRvec,cv::Mat srcTvec,cv::Mat dstRvec,cv::Mat dstTvec,cv::Mat& src2dstRvec,cv::Mat&src2dstTvec)
{
    srcRvec.convertTo(srcRvec,CV_64F);
    srcTvec.convertTo(srcTvec,CV_64F);
    dstRvec.convertTo(dstRvec,CV_64F);
    dstTvec.convertTo(dstTvec,CV_64F);
    cv::Mat matrixSrc=cv::Mat::zeros(3,3,CV_64F);
    cv::Mat InvMatrixSrc=cv::Mat::zeros(3,3,CV_64F);
    cv::Mat matrixDst=cv::Mat::zeros(3,3,CV_64F);
    cv::Mat R=cv::Mat::zeros(3,3,CV_64F);
    cv::Rodrigues(srcRvec,matrixSrc);
    cv::Rodrigues(dstRvec,matrixDst);
    InvMatrixSrc=matrixSrc.inv();
    cv::gemm(matrixDst,InvMatrixSrc,1,NULL,0,R,0);
    cv::gemm(R,srcTvec,-1,dstTvec,1,src2dstTvec,0);
    cv::Rodrigues(R,src2dstRvec,cv::noArray());
}
void StereoCalibrator::calRelative()
{
    std::cout<<"begin cal R T..."<<std::endl<<std::flush;
    std::vector<std::vector<cv::Point3f> > objectPoint;
    std::vector<std::vector<cv::Point2f> > imagePoint1;
    std::vector<std::vector<cv::Point2f> > imagePoint2;
    size_t lt=srcCalibrator.getImages().size();
    size_t lr=dstCalibrator.getImages().size();
    size_t num=std::min(lt,lr);
//    std::vector<bool> srcUsed=srcCalibrator.getUsed();
//    std::vector<bool> dstUsed=dstCalibrator.getUsed();
//    CalibrateResult srcResult=srcCalibrator.getResult();
//    CalibrateResult dstResult=dstCalibrator.getResult();
//    std::vector<cv::Mat> src2dstRvecs;
//    std::vector<cv::Mat> src2dstTvecs;
//    for(int i=0;i<num;i++)
//    {
//        if(srcUsed[i]&&dstUsed[i]){
//            cv::Mat src2dstRvec;
//            cv::Mat src2dstTvec;
//            calsrc2dstRt(srcResult.rvecs[i],srcResult.tvecs[i],dstResult.rvecs[i],dstResult.tvecs[i],src2dstRvec,src2dstTvec);
//            src2dstRvecs.push_back(src2dstRvec);
//            src2dstTvecs.push_back(src2dstTvec);
//        }
//    }
//    num=src2dstRvecs.size();
    std::vector<bool> srcUsed=srcCalibrator.getUsed();
    std::vector<bool> dstUsed=dstCalibrator.getUsed();
    const std::vector<std::vector<cv::Point3f> >& srcObjects=srcCalibrator.getObjectPoints();
    const std::vector<std::vector<cv::Point2f>>& srcImagePoints=srcCalibrator.getImagePoints();
    const std::vector<std::vector<cv::Point2f>>& dstImagePoints=dstCalibrator.getImagePoints();
    for(int i=0;i<num;i++)
    {
        if(srcUsed[i]&&dstUsed[i]){
            objectPoint.push_back((srcObjects[i]));
            imagePoint1.push_back(srcImagePoints[i]);
            imagePoint2.push_back(dstImagePoints[i]);
        }
    }
    CalibrateResult srcResult=srcCalibrator.getResult();
    CalibrateResult dstResult=dstCalibrator.getResult();
    cv::Mat R,T,E,F;
    /*
        进行立体双目标定
        由于左右摄像机分别都经过了单目标定
        所以在此处选择flag = CALIB_USE_INTRINSIC_GUESS
        */
        double rms = cv::stereoCalibrate(objectPoint,   //vector<vector<point3f>> 型的数据结构，存储标定角点在世界坐标系中的位置
            imagePoint1,                             //vector<vector<point2f>> 型的数据结构，存储标定角点在第一个摄像机下的投影后的亚像素坐标
            imagePoint2,                             //vector<vector<point2f>> 型的数据结构，存储标定角点在第二个摄像机下的投影后的亚像素坐标
            srcResult.instrisincMatrix,                           //输入/输出型的第一个摄像机的相机矩阵。如果CV_CALIB_USE_INTRINSIC_GUESS , CV_CALIB_FIX_ASPECT_RATIO ,CV_CALIB_FIX_INTRINSIC , or CV_CALIB_FIX_FOCAL_LENGTH其中的一个或多个标志被设置，该摄像机矩阵的一些或全部参数需要被初始化
            srcResult.distortionCoeff,                              //第一个摄像机的输入/输出型畸变向量。根据矫正模型的不同，输出向量长度由标志决定
            dstResult.instrisincMatrix,                           //输入/输出型的第二个摄像机的相机矩阵。参数意义同第一个相机矩阵相似
            dstResult.distortionCoeff,                              //第一个摄像机的输入/输出型畸变向量。根据矫正模型的不同，输出向量长度由标志决定
            srcResult.imageSize,           //图像的大小
            R,                                       //输出型，第一和第二个摄像机之间的旋转矩阵
            T,                                       //输出型，第一和第二个摄像机之间的平移矩阵
            E,                                       //输出本征矩阵
            F,                                       //输出基础矩阵
            cv::CALIB_USE_INTRINSIC_GUESS,
            cv::TermCriteria(cv::TermCriteria::COUNT + cv::TermCriteria::EPS, 100, 1e-5));
       srcResult.saveCameraParams(srcCalibrator.getOutputFileName(),srcCalibrator.getCalibrationFlags());
       dstResult.saveCameraParams(dstCalibrator.getOutputFileName(),dstCalibrator.getCalibrationFlags());
       CalibrateResult relative;
       relative.R=R;
       relative.T=T;
       relative.rms=rms;
       relative.saveRelativeParams(relativeOutputFileName,calibrationFlags);
       std::cout<<"stereo calibrate done!"<<std::endl<<std::flush;
}

std::string StereoCalibrator::getRelativeOutputFileName() const
{
    return relativeOutputFileName;
}

void StereoCalibrator::setRelativeOutputFileName(const std::string &value)
{
    relativeOutputFileName = value;
}
