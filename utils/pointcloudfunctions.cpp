#include "pointcloudfunctions.h"

PointCloudFunctions::PointCloudFunctions()
{

}
bool PointCloudFunctions::saveXYZRGBPointCloud(const cv::Mat &cloud, std::string filename)
{
    std::ofstream out(filename);
    if(!out){
        std::cerr<<"write file:"<<filename<<" fail!"<<std::endl<<std::flush;
        return false;
    }
    if(cloud.empty()){
        std::cerr<<"cloud is empty!"<<std::endl<<std::flush;
        return false;
    }
    int i, j;
    int count=0;
    for (i = 0; i < cloud.rows; i++) {
        for (j = 0; j < cloud.cols; j++) {
            cv::Vec4f v=cloud.at<cv::Vec4f>(i, j);
            if (std::isnan(v[0]) || std::isnan(v[1]) || std::isnan(v[2]))continue;
            if (std::isinf(v[0]) || std::isinf(v[1]) || std::isinf(v[2]))continue;
            out << "v " << v[0]<<" " << v[1] << " " << v[2] << " ";
            unsigned char* temp= (unsigned char*)(&v[3]);
            out << (int)temp[0] << " " << (int)temp[1] << " " << (int)temp[2]<< " " << (int)temp[3];
            out<< std::endl;
            count++;
        }
    }
    out.close();
    std::cout<<"save "<<count<<" points!"<<std::endl<<std::flush;
    return count>0;
}

bool PointCloudFunctions::saveXYZRGBPointCloud(std::vector<cv::Vec4f> &cloud, std::string filename)
{
    std::ofstream out(filename);
    if(!out){
        std::cerr<<"write file:"<<filename<<" fail!"<<std::endl<<std::flush;
        return false;
    }
    if(cloud.empty()){
        std::cerr<<"cloud is empty!"<<std::endl<<std::flush;
        return false;
    }
    int i, j;
    int count=0;
    for (i = 0; i < cloud.size(); i++) {
        cv::Vec4f v=cloud[i];
        if (std::isnan(v[0]) || std::isnan(v[1]) || std::isnan(v[2]))continue;
        if (std::isinf(v[0]) || std::isinf(v[1]) || std::isinf(v[2]))continue;
        out << "v " << v[0]<<" " << v[1] << " " << v[2] << " ";
        unsigned char* temp= (unsigned char*)(&v[3]);
        out << (int)temp[0] << " " << (int)temp[1] << " " << (int)temp[2]<< " " << (int)temp[3];
        out<< std::endl;
        count++;
    }
    out.close();
    std::cout<<"save "<<count<<" points!"<<std::endl<<std::flush;
    return count>0;
}

bool PointCloudFunctions::loadXYZRGBPointCloud(std::vector<cv::Vec4f>& data, std::string filename)
{
    std::ifstream in(filename.c_str());

    if(!in.good())
    {
        std::cout  << "ERROR: loading obj:(" << filename << ") file is not good" << "\n"<<std::flush;
        return false;
    }

    char buffer[256], str[255];
    float f1,f2,f3;
    unsigned char color[4];
    ;
    while(!in.getline(buffer,255).eof())
    {
        buffer[255]='\0';

        sscanf_s(buffer,"%s",str,255);
        cv::Vec4f v;
        // reading a vertex
        if (buffer[0]=='v' && (buffer[1]==' '  || buffer[1]==32) )
        {
            if ( sscanf(buffer,"v %f %f %f %i %i %i %i",&f1,&f2,&f3,color+0,color+1,color+2,color+3)==7)
            {
                v[0]=f1;v[1]=f2;v[2]=f3;
                v[4]=*((float*)color);
                data.push_back(v);
            }
            else
            {
                std::cout << "ERROR: vertex not in wanted format in OBJLoader" << "\n";
                exit(-1);
            }
        }
        // reading FaceMtls
        else if (buffer[0]=='f' && (buffer[1]==' ' || buffer[1]==32) )
        {

        }
    }

}

void PointCloudFunctions::applyTransformXYZRGBPointCloud(const cv::Mat &cloud, const cv::Mat &R, const cv::Mat &T,cv::Mat& out)
{
    out=cloud.clone();
    int i, j;
    for (i = 0; i < cloud.rows; i++) {
        for (j = 0; j < cloud.cols; j++) {
            cv::Vec4f v=cloud.at<cv::Vec4f>(i, j);
            cv::Vec4f& r=out.at<cv::Vec4f>(i, j);
            r[0]=R.at<double>(0,0)*v[0]+R.at<double>(0,1)*v[1]+R.at<double>(0,2)*v[2]+T.at<double>(0,0);
            r[1]=R.at<double>(1,0)*v[0]+R.at<double>(1,1)*v[1]+R.at<double>(1,2)*v[2]+T.at<double>(1,0);
            r[2]=R.at<double>(2,0)*v[0]+R.at<double>(2,1)*v[1]+R.at<double>(2,2)*v[2]+T.at<double>(2,0);
        }
    }
}

void PointCloudFunctions::applyTransformXYZRGBPointCloud(std::vector<cv::Vec4f> &cloud, const cv::Mat &R, const cv::Mat &T, std::vector<cv::Vec4f> &out)
{
    out.assign(cloud.begin(),cloud.end());
    int i, j;
    for (i = 0; i < cloud.size(); i++) {

            cv::Vec4f v=cloud[i];
            cv::Vec4f& r=out[i];
            r[0]=R.at<double>(0,0)*v[0]+R.at<double>(0,1)*v[1]+R.at<double>(0,2)*v[2]+T.at<double>(0,0);
            r[1]=R.at<double>(1,0)*v[0]+R.at<double>(1,1)*v[1]+R.at<double>(1,2)*v[2]+T.at<double>(1,0);
            r[2]=R.at<double>(2,0)*v[0]+R.at<double>(2,1)*v[1]+R.at<double>(2,2)*v[2]+T.at<double>(2,0);
    }
}
