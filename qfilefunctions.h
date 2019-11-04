#ifndef QFILEFUNCTIONS_H
#define QFILEFUNCTIONS_H

#include <QString>
class QFileFunctions
{
public:
    QFileFunctions();
    static void clearFiles(const QString &folderFullPath);
    static void clearFolder(const QString &folderFullPath,bool deleteItself);
};

#endif // QFILEFUNCTIONS_H
