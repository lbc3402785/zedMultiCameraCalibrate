#ifndef ZEDCAMERATHREAD_H
#define ZEDCAMERATHREAD_H
#include <string>
#include <QThread>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <sl/Camera.hpp>
#include "utils/stereocalibrator.h"
#include "threadsafequeue.h"
#include "settings.h"
#include "eigenmesh.h"
#include <atomic>
class ZedCameraThread: public QThread
{
    Q_OBJECT
public:
    bool detected;
    bool detect;
    ZedCameraThread(QObject *parent = 0);
    ~ZedCameraThread();
    enum SaveMode{IMAGE,CLOUD,DEPTH};
    // QThread interface
    int getId() const;
    void setId(int value);
    bool readZedData(sl::Camera* zed,cv::Mat& out,SaveMode mode,bool wait=false);
    bool readZedImage(sl::Camera* zed,cv::Mat& image,bool wait=false);
    bool readZedDepth(sl::Camera* zed,cv::Mat& depth,bool wait=false);
    bool readZedCloud(sl::Camera* zed,cv::Mat& cloud,bool wait=false);
    bool detectAndDrawCorners(cv::Mat& image,cv::Mat& grayImage,cv::Mat& drawImage);
    void readCamera();
    ThreadSafeQueue<QPixmap> *getShowQueue() const;
    void setShowQueue(ThreadSafeQueue<QPixmap> *value);
    sl::Camera*zed;


    void saveZedData(QString imageDir,QString drawDir,QString outputDir);
    void saveDepth(cv::Mat depth,std::string savePath);
    void saveCloud(cv::Mat cloud,EigenMesh& mesh,std::string savePath);
    SaveMode getSaveMode() const;
    void setSaveMode(const SaveMode &value);

    bool getDetect() const;
    void setDetect(bool value);

    bool getStopSignal() const;
    void setStopSignal(bool value);

    bool getCapturing() const;
    void setCapturing(bool value);

    QString getName() const;
    void setName(const QString &value);

    bool getPause() const;
    void setPause(bool value);
    std::vector<cv::Mat> mats;
    std::vector<cv::Mat> grayMats;
    std::atomic<int> num;
    EigenMesh getMesh() const;
    void setMesh(const EigenMesh &value);
    void convertEigenMesh(const cv::Mat& cloud, EigenMesh &mesh);
public slots:
    void close();
    void saveCalibrateData();
protected:
    void run();
private:
    QString name;
    int id;
    ThreadSafeQueue<QPixmap>* showQueue;
    bool useSDKParam;
    bool useRectified;


    bool check;
    bool capturing;
    SaveMode saveMode;
    bool stopSignal;
    bool pause;
    cv::Mat image;
    cv::Mat imageGrayMat;
    cv::Mat imageDrawMat;
    cv::Mat depth;
    EigenMesh mesh;
    Settings& sets=Settings::instance();
    float depthFactor;



signals:
    void returnQPixmap();
    void capturedSuccessful();
};

#endif // ZEDCAMERATHREAD_H
