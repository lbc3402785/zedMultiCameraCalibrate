#include "stereocalibrator.h"
template<class T>
std::vector<T> SubVector(std::vector<T> input, int start, int len = 1)
{
    auto first = input.begin() + start;
    auto last = input.begin() + start + len;
    std::vector<T> newVec(first, last);
    return newVec;
}
StereoCalibrator::StereoCalibrator()
{
    calibrationFlags=0;
    eps=0.85;
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
bool isRotationMatrix(cv::Mat &R)
{
    cv::Mat Rt;
    transpose(R, Rt);
    cv::Mat shouldBeIdentity = Rt * R;
    cv::Mat I = cv::Mat::eye(3,3, shouldBeIdentity.type());

    return  norm(I, shouldBeIdentity) < 1e-6;

}
cv::Mat rot2euler(const cv::Mat & rotationMatrix)
{
    cv::Mat euler(3,1,CV_64F);

    double m00 = rotationMatrix.at<double>(0,0);
    double m02 = rotationMatrix.at<double>(0,2);
    double m10 = rotationMatrix.at<double>(1,0);
    double m11 = rotationMatrix.at<double>(1,1);
    double m12 = rotationMatrix.at<double>(1,2);
    double m20 = rotationMatrix.at<double>(2,0);
    double m22 = rotationMatrix.at<double>(2,2);

    double x, y, z;

    // Assuming the angles are in radians.
    if (m10 > 0.998) { // singularity at north pole
        x = 0;
        y = CV_PI/2;
        z = atan2(m02,m22);
    }
    else if (m10 < -0.998) { // singularity at south pole
        x = 0;
        y = -CV_PI/2;
        z = atan2(m02,m22);
    }
    else
    {
        x = atan2(-m12,m11);
        y = asin(m10);
        z = atan2(-m20,m00);
    }

    euler.at<double>(0) = x;
    euler.at<double>(1) = y;
    euler.at<double>(2) = z;

    return euler;
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
    src2dstRvec=cv::Mat::zeros(3,1,CV_64F);
    src2dstTvec=cv::Mat::zeros(3,1,CV_64F);
    cv::Rodrigues(srcRvec,matrixSrc);
    cv::Rodrigues(dstRvec,matrixDst);
    InvMatrixSrc=matrixSrc.inv();
    cv::gemm(matrixDst,InvMatrixSrc,1,NULL,0,R,0);
    cv::gemm(R,srcTvec,-1,dstTvec,1,src2dstTvec,0);
    cv::Rodrigues(R,src2dstRvec,cv::noArray());
    std::cout<<"R:"<<R<<std::endl<<std::flush;
    std::cout<<"src2dstTvec:"<<src2dstTvec<<std::endl<<std::flush;
}
void StereoCalibrator::calRelative()
{
    using namespace cv;
    using namespace std;
    std::cout<<"begin cal R T..."<<std::endl<<std::flush;
    std::vector<std::vector<cv::Point3f> > objectPoints;
    std::vector<std::vector<cv::Point2f> > imagePoint1;
    std::vector<std::vector<cv::Point2f> > imagePoint2;
    size_t lt=srcCalibrator.getImages().size();
    size_t lr=dstCalibrator.getImages().size();
    size_t num=std::min(lt,lr);
    std::vector<bool> srcUsed=srcCalibrator.getUsed();
    std::vector<bool> dstUsed=dstCalibrator.getUsed();
    CalibrateResult srcResult=srcCalibrator.getResult();
    CalibrateResult dstResult=dstCalibrator.getResult();


    if(!useSDKParam){
        std::cout<<"calsrc2dstRt"<<std::endl<<std::flush;
        std::vector<cv::Mat> src2dstRvecs;
        std::vector<cv::Mat> src2dstTvecs;
        for(int i=0;i<num;i++)
        {
            if(srcUsed[i]&&dstUsed[i]){
                cv::Mat src2dstRvec;
                cv::Mat src2dstTvec;
                calsrc2dstRt(srcResult.rvecs[i],srcResult.tvecs[i],dstResult.rvecs[i],dstResult.tvecs[i],src2dstRvec,src2dstTvec);
                src2dstRvecs.push_back(src2dstRvec);
                src2dstTvecs.push_back(src2dstTvec);
            }
        }
    }
    std::cout<<"begin fix data..."<<std::endl<<std::flush;
    //    num=src2dstRvecs.size();
    const std::vector<std::vector<cv::Point3f> >& srcfullObjects=srcCalibrator.getFullObjectPoints();
    const std::vector<std::vector<cv::Point2f>>& srcfullImagePoints=srcCalibrator.getFullImagePoints();
    const std::vector<std::vector<cv::Point2f>>& dstfullImagePoints=dstCalibrator.getFullImagePoints();
    for(int i=0;i<num;i++)
    {
        if(srcUsed[i]&&dstUsed[i]){
            objectPoints.push_back((srcfullObjects[i]));
            imagePoint1.push_back(srcfullImagePoints[i]);
            imagePoint2.push_back(dstfullImagePoints[i]);
        }
    }
    std::cout<<"fix data done!"<<std::endl<<std::flush;
    auto DISTORTIONFLAG = CALIB_FIX_K3 + CALIB_FIX_K4 + CALIB_FIX_K5 + CALIB_FIX_K6;
    if (useSDKParam)
    {
        DISTORTIONFLAG = CALIB_FIX_K1 + CALIB_FIX_K2 + CALIB_FIX_K3 + CALIB_FIX_K4 + CALIB_FIX_K5 + CALIB_FIX_K6 + CALIB_FIX_ASPECT_RATIO;
    }
    cv::Mat R,T,E,F;
    std::vector<std::vector<cv::Point3f> > objectPoints2;
    std::vector<std::vector<cv::Point2f> > imagePoint11;
    std::vector<std::vector<cv::Point2f> > imagePoint21;
    for (size_t i = 0; i < objectPoints.size(); i++)
    {

        double rms = cv::stereoCalibrate(SubVector(objectPoints, i),
                                         SubVector(imagePoint1, i),
                                         SubVector(imagePoint2, i),
                                         srcResult.intrinsicMatrix,
                                         srcResult.distortionCoeff,
                                         dstResult.intrinsicMatrix,
                                         dstResult.distortionCoeff,
                                         srcResult.imageSize,
                                         R,
                                         T,
                                         E,
                                         F,
                                         CALIB_FIX_INTRINSIC |
                                         CALIB_FIX_ASPECT_RATIO |
                                         CALIB_RATIONAL_MODEL |
                                         DISTORTIONFLAG
                                         ,
                                         TermCriteria(TermCriteria::COUNT + TermCriteria::EPS, 100, 1e-5));
        cout << i << ": RMS error=" << rms << T << endl;

        if (rms < eps)
        {
            objectPoints2.push_back(objectPoints[i]);
            imagePoint11.push_back(imagePoint1[i]);
            imagePoint21.push_back(imagePoint2[i]);
        }
    }

    cout << "select size:" << objectPoints2.size() << endl;
    if(objectPoints2.size()<5){
        cerr << "so little valid image!" << endl;
    }

    /*
        进行立体双目标定
        由于左右摄像机分别都经过了单目标定
        所以在此处选择flag = CALIB_USE_INTRINSIC_GUESS
        */
    double rms = cv::stereoCalibrate(/*objectPoints*/objectPoints2,   //vector<vector<point3f>> 型的数据结构，存储标定角点在世界坐标系中的位置
                                     /*imagePoint1*/imagePoint11,                             //vector<vector<point2f>> 型的数据结构，存储标定角点在第一个摄像机下的投影后的亚像素坐标
                                     /*imagePoint2*/imagePoint21,                             //vector<vector<point2f>> 型的数据结构，存储标定角点在第二个摄像机下的投影后的亚像素坐标
                                     srcResult.intrinsicMatrix,                           //输入/输出型的第一个摄像机的相机矩阵。如果CV_CALIB_USE_INTRINSIC_GUESS , CV_CALIB_FIX_ASPECT_RATIO ,CV_CALIB_FIX_INTRINSIC , or CV_CALIB_FIX_FOCAL_LENGTH其中的一个或多个标志被设置，该摄像机矩阵的一些或全部参数需要被初始化
                                     srcResult.distortionCoeff,                              //第一个摄像机的输入/输出型畸变向量。根据矫正模型的不同，输出向量长度由标志决定
                                     dstResult.intrinsicMatrix,                           //输入/输出型的第二个摄像机的相机矩阵。参数意义同第一个相机矩阵相似
                                     dstResult.distortionCoeff,                              //第一个摄像机的输入/输出型畸变向量。根据矫正模型的不同，输出向量长度由标志决定
                                     srcResult.imageSize,           //图像的大小
                                     R,                                       //输出型，第一和第二个摄像机之间的旋转矩阵
                                     T,                                       //输出型，第一和第二个摄像机之间的平移矩阵
                                     E,                                       //输出本征矩阵
                                     F,                                       //输出基础矩阵
                                     (useSDKParam?cv::CALIB_FIX_INTRINSIC:cv::CALIB_USE_INTRINSIC_GUESS)|DISTORTIONFLAG|CALIB_ZERO_TANGENT_DIST|CALIB_RATIONAL_MODEL,
                                     cv::TermCriteria(cv::TermCriteria::COUNT + cv::TermCriteria::EPS, 100, 1e-6));
    srcResult.saveCameraParams(srcCalibrator.getOutputFileName(),srcCalibrator.getCalibrationFlags());
    dstResult.saveCameraParams(dstCalibrator.getOutputFileName(),dstCalibrator.getCalibrationFlags());
    relative.R=R;
    relative.T=T;
    relative.rms=rms;
    relative.saveRelativeParams(relativeOutputFileName,calibrationFlags);
    std::cout<<"R:"<<R<<std::endl<<std::flush;\
    std::cout<<"rms:"<<rms<<std::endl<<std::flush;
    //std::cout<<"euler:"<<rot2euler(R)<<std::endl<<std::flush;
    std::cout<<"T:"<<T<<std::endl<<std::flush;
    std::cout<<"stereo calibrate done!"<<std::endl<<std::flush;
}

bool StereoCalibrator::getUseSDKParam() const
{
    return useSDKParam;
}

void StereoCalibrator::setUseSDKParam(bool value)
{
    useSDKParam = value;
}

std::string StereoCalibrator::getRelativeOutputFileName() const
{
    return relativeOutputFileName;
}

void StereoCalibrator::setRelativeOutputFileName(const std::string &value)
{
    relativeOutputFileName = value;
}

CalibrateResult StereoCalibrator::getRelative() const
{
    return relative;
}

void StereoCalibrator::setRelative(const CalibrateResult &value)
{
    relative = value;
}
