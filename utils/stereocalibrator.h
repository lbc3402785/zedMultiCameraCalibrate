#ifndef STEREOCALIBRATOR_H
#define STEREOCALIBRATOR_H
#include <QString>
#include "calibrator.h"
class StereoCalibrator
{
public:
    StereoCalibrator();

    void calibrateStereo();
    Calibrator getSrcCalibrator() const;
    void setSrcCalibrator(const Calibrator &value);

    Calibrator getDstCalibrator() const;
    void setDstCalibrator(const Calibrator &value);

    std::string getRelativeOutputFileName() const;
    void setRelativeOutputFileName(const std::string &value);

private:
    int calibrationFlags;
    Calibrator srcCalibrator;
    Calibrator dstCalibrator;
    void calRelative();
    std::string relativeOutputFileName;
};

#endif // STEREOCALIBRATOR_H
