#include "settings.h"
#include <QSettings>
#include <QDir>
#include <QStandardPaths>
Settings::Settings()
{
    loadSettings();
}

void Settings::loadSettings()
{
    QSettings settings(SETTINGS_FILE);
    chessBoardConfig.boardW=settings.value("boardW",DefaultBoardW).value<int>();
    chessBoardConfig.boardH=settings.value("boardH",DefaultBoardH).value<int>();
    chessBoardConfig.dx=settings.value("dx",DefaultDx).value<double>();
    chessBoardConfig.dy=settings.value("dy",DefaultDy).value<double>();
    imageDir=settings.value("imageDir", getDefaultImageDir()).toString();
    drawDir=settings.value("drawDir", getDefaultDrawDir()).toString();
    outputDir=settings.value("outputDir", getDefaultOutputDir()).toString();
    imageSurffix=settings.value("imageSurffix", "bmp").toString();
}

QString Settings::getDefaultImageDir()
{
#if defined (Q_OS_WIN)
    return
        QStandardPaths::locate(QStandardPaths::DocumentsLocation, QString(), QStandardPaths::LocateDirectory) + "captureThreeImage";
#else
    return QDir::homePath() + QDir::separator() + "stereoimage";
#endif
}

QString Settings::getDefaultDrawDir()
{
#if defined (Q_OS_WIN)
    return
        QStandardPaths::locate(QStandardPaths::DocumentsLocation, QString(), QStandardPaths::LocateDirectory) + "captureThreeDraw";
#else
    return QDir::homePath() + QDir::separator() + "draw";
#endif
}

QString Settings::getDefaultOutputDir()
{
#if defined (Q_OS_WIN)
    return
        QStandardPaths::locate(QStandardPaths::DocumentsLocation, QString(), QStandardPaths::LocateDirectory) + "captureThreeOutput";
#else
    return QDir::homePath() + QDir::separator() + "output";
#endif
}

QString Settings::getDefaultImageSurffix()
{
    return ".bmp";
}

QString Settings::getOutputDir() const
{
    return outputDir;
}

void Settings::setOutputDir(const QString &value)
{
    outputDir = value;
}

QString Settings::getImageSurffix() const
{
    return imageSurffix;
}

void Settings::setImageSurffix(const QString &value)
{
    imageSurffix = value;
}

QString Settings::getDrawDir() const
{
    return drawDir;
}

void Settings::setDrawDir(const QString &value)
{
    drawDir = value;
}

QString Settings::getImageDir() const
{
    return imageDir;
}

void Settings::setImageDir(const QString &value)
{
    imageDir = value;
}

void Settings::saveSettings()
{
    QSettings settings(SETTINGS_FILE);
    settings.setValue("boardW", chessBoardConfig.boardW);
    settings.setValue("boardH", chessBoardConfig.boardH);
    settings.setValue("dx", chessBoardConfig.dx);
    settings.setValue("dy", chessBoardConfig.dy);
    settings.setValue("imageDir", imageDir);
    settings.setValue("drawDir", drawDir);
    settings.setValue("imageSurffix", imageSurffix);
    settings.setValue("outputDir", outputDir);
}

void Settings::reset()
{
    chessBoardConfig.boardW=DefaultBoardW;
    chessBoardConfig.boardH=DefaultBoardH;
    chessBoardConfig.dx=DefaultDx;
    chessBoardConfig.dy=DefaultDy;
    imageDir=getDefaultImageDir();
    drawDir=getDefaultDrawDir();
    imageSurffix=getDefaultImageSurffix();
    outputDir=getDefaultOutputDir();
}


