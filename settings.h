#ifndef SETTINGS_H
#define SETTINGS_H
#include <QString>
#include "chessboardconfig.h"

const int DefaultBoardW=9;
const int DefaultBoardH=7;
const double DefaultDx=20.0;
const double DefaultDy=20.0;
const QString SETTINGS_FILE{"ChessboardConfig"};
class Settings
{
public:
    static Settings& instance() {static Settings obj; return obj; }
    void saveSettings();
    void reset();
    QString getImageDir() const;
    void setImageDir(const QString &value);

    QString getDrawDir() const;
    void setDrawDir(const QString &value);

    QString getImageSurffix() const;
    void setImageSurffix(const QString &value);

    QString getOutputDir() const;
    void setOutputDir(const QString &value);
    ChessBoardConfig chessBoardConfig;
private:
    Settings();
    void loadSettings();
    QString getDefaultImageDir();
    QString getDefaultDrawDir();
    QString getDefaultOutputDir();
    QString getDefaultImageSurffix();
    QString imageDir;
    QString drawDir;
    QString imageSurffix;
    QString outputDir;
};

#endif // SETTINGS_H
