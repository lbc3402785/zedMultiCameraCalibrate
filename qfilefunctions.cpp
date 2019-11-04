#include "qfilefunctions.h"
#include <QDir>
QFileFunctions::QFileFunctions()
{

}
/********************************************************************************
*说明：删除文件及文件夹涉及到的类包括QDir、QFile、QFileInfo、QFileInfoList，
*     清空文件夹的方法很多，此处仅给出我常用的两种方法
*********************************************************************************/



/********************************************************************************

*clearFiles():仅清空文件夹内的文件(不包括子文件夹内的文件)

*folderFullPath:文件夹全路径

*********************************************************************************/

void QFileFunctions::clearFiles(const QString &folderFullPath)

{

    QDir dir(folderFullPath);

    dir.setFilter(QDir::Files);

    int fileCount = dir.count();

    for (int i = 0; i < fileCount; i++)

        dir.remove(dir[i]);
}



/********************************************************************************

* clearFolder():删除非空文件夹

* folderFullPath:文件夹全路径

*********************************************************************************/

void QFileFunctions::clearFolder(const QString &folderFullPath,bool deleteItself)

{
    QDir             dir(folderFullPath);

    QFileInfoList    fileList;

    QFileInfo        curFile;

    QFileInfoList    fileListTemp;

    int32_t            infoNum;

    int32_t            i;

    int32_t            j;

    /* 首先获取目标文件夹内所有文件及文件夹信息 */

    fileList=dir.entryInfoList(QDir::Dirs|QDir::Files

                               |QDir::Readable|QDir::Writable

                               |QDir::Hidden|QDir::NoDotAndDotDot

                               ,QDir::Name);
    while(fileList.size() > 0)

    {
        infoNum = fileList.size();
        for(i = infoNum - 1; i >= 0; i--)
        {
            curFile = fileList[i];
            if(curFile.isFile()) /* 如果是文件，删除文件 */
            {
                QFile fileTemp(curFile.filePath());
                fileTemp.remove();
                fileList.removeAt(i);
            }



            if(curFile.isDir()) /* 如果是文件夹 */

            {

                QDir dirTemp(curFile.filePath());

                fileListTemp = dirTemp.entryInfoList(QDir::Dirs | QDir::Files

                                                     | QDir::Readable | QDir::Writable

                                                     | QDir::Hidden | QDir::NoDotAndDotDot

                                                     , QDir::Name);

                if(fileListTemp.size() == 0) /* 下层没有文件或文件夹 则直接删除 */

                {
                    dirTemp.rmdir(".");

                    fileList.removeAt(i);
                }

                else /* 下层有文件夹或文件 则将信息添加到列表 */
                {
                    for(j = 0; j < fileListTemp.size(); j++)
                    {
                        if(!(fileList.contains(fileListTemp[j])))
                        {
                            fileList.append(fileListTemp[j]);
                        }
                    }
                }
            }
        }
    }

    if(deleteItself)dir.rmdir(".");/* 删除目标文件夹,如果只是清空文件夹folderFullPath的内容而不删除folderFullPath本身,则删掉本行即可 */

}
