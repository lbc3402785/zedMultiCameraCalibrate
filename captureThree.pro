#-------------------------------------------------
#
# Project created by QtCreator 2019-10-22T17:58:36
#
#-------------------------------------------------

QT       += core gui
QT += multimedia
QT       *= xml opengl
QMAKE_CXXFLAGS+=/bigobj
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = captureThree
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS _USE_MATH_DEFINES GLOG_NO_ABBREVIATED_SEVERITIES

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11
OBJECTS_DIR=$${PWD}/build
OPENCV_INCLUDE_DIRS=D:\soft\opencv3\build\include
OPENCV_LIBRARY_DIRS=D:\soft\opencv3\build\x64\vc15\lib
CGAL_INCLUDE_DIRS="C:\Program Files (x86)\CGAL\include"
CGAL_LIBRARY_DIRS="C:\Program Files (x86)\CGAL\lib"
OPENMESH_INCLUDE_DIRS="D:\soft\OpenMesh8.0\include"
OPENMESH_LIBRARY_DIRS="D:\soft\OpenMesh8.0\lib"
QGLVIEWER_INCLUDE_DIRS="D:\soft\libQGLViewer-2.7.1"
QGLVIEWER_LIBRARY_DIRS="D:\soft\libQGLViewer-2.7.1\QGLViewer"
TRIMESH_INCLUDE_DIRS="D:\soft\trimesh2\include"
TRIMESH_LIBRARY_DIRS="D:\soft\trimesh2\lib"
GMP_LIBRARY_DIRS="D:\soft\CGAL-4.14.1\auxiliary\gmp\lib"
GMP_INCLUDE_DIRS="D:\soft\CGAL-4.14.1\auxiliary\gmp\include"
ZLIB_INCLUDE_DIRS="D:\soft\opencv3\sources\3rdparty\zlib"
FLANN_LIBRARY_DIRS=D:\soft\flann-1.8.4-src\lib
FLANN_INCLUDE_DIRS=D:\soft\flann-1.8.4-src\src\cpp

INCLUDEPATH+="C:\Program Files (x86)\ZED SDK\include"
INCLUDEPATH+="C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v10.0\include"
LIBS+=-L"C:\Program Files (x86)\ZED SDK\lib" -lsl_zed64
LIBS+=-L"C:\Program Files (x86)\ZED SDK\lib" -lsl_input64
LIBS+=-L"C:\Program Files (x86)\ZED SDK\lib" -lsl_core64
LIBS+="C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v10.0\lib\x64\cuda.lib"
LIBS+="C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v10.0\lib\x64\cudart.lib"

INCLUDEPATH+=D:\soft\eigen-eigen-323c052e1731
INCLUDEPATH+=$$TRIMESH_INCLUDE_DIRS
INCLUDEPATH+=$$QGLVIEWER_INCLUDE_DIRS
INCLUDEPATH+=$$CGAL_INCLUDE_DIRS
INCLUDEPATH+=$$GMP_INCLUDE_DIRS
INCLUDEPATH+=$$(DLIB_ROOT)\include
INCLUDEPATH+=$$OPENCV_INCLUDE_DIRS
INCLUDEPATH+=$$OPENMESH_INCLUDE_DIRS
INCLUDEPATH+=$$ZLIB_INCLUDE_DIRS
INCLUDEPATH+=$$FLANN_INCLUDE_DIRS

LIBS+=-L$$OPENCV_LIBRARY_DIRS -lopencv_world344
LIBS+=-L$$OPENCV_LIBRARY_DIRS -lopencv_ximgproc344
LIBS+=-L$$OPENCV_LIBRARY_DIRS -lopencv_calib3d344
LIBS+=-L$$OPENCV_LIBRARY_DIRS -lopencv_features2d344
LIBS+=-L$$OPENCV_LIBRARY_DIRS -lopencv_flann344
LIBS+=-L$$OPENCV_LIBRARY_DIRS -lopencv_highgui344
LIBS+=-L$$OPENCV_LIBRARY_DIRS -lopencv_videoio344
LIBS+=-L$$OPENCV_LIBRARY_DIRS -lopencv_imgcodecs344
LIBS+=-L$$OPENCV_LIBRARY_DIRS -lopencv_core344
LIBS += -L$$TRIMESH_LIBRARY_DIRS   -ltrimesh2
LIBS += -L$$TRIMESH_LIBRARY_DIRS   -lgluit
LIBS +=-L$$OPENMESH_LIBRARY_DIRS -lOpenMeshCore
LIBS +=-L$$QGLVIEWER_LIBRARY_DIRS -lQGLViewer2
LIBS+=-L$$(DLIB_ROOT)\lib -ldlib19.16.99_release_64bit_msvc1900
LIBS+=-L$$OPENCV_LIBRARY_DIRS -lopencv_world344
LIBS +=-L$$CGAL_LIBRARY_DIRS -llibCGAL_Core-vc141-mt-4.14.1 -llibCGAL-vc141-mt-4.14.1
LIBS+=-L$$GMP_LIBRARY_DIRS -llibgmp-10
LIBS+=-L$$FLANN_LIBRARY_DIRS -lflann

INCLUDEPATH+="C:\Program Files (x86)\Ceres\include" \
                 "C:\Program Files (x86)\glog\include" \
                 "C:\Program Files (x86)\gflags\include"

LIBS+=-L"C:\Program Files (x86)\Ceres\lib" -lceres
LIBS+=-L"C:\Program Files (x86)\glog\lib" -lglog
LIBS+=-L"C:\Program Files (x86)\gflags\lib" -lgflags


SOURCES += \
        chessboardconfig.cpp \
        dialog.cpp \
        eigenmesh.cpp \
        graphics/viewer.cpp \
        main.cpp \
        mainwindow.cpp \
        modelwindow.cpp \
        qfilefunctions.cpp \
        settings.cpp \
        utils/calibrateresult.cpp \
        utils/calibrator.cpp \
        utils/stereocalibrator.cpp

HEADERS += \
        chessboardconfig.h \
        dialog.h \
        eigenmesh.h \
        graphics/viewer.h \
        mainwindow.h \
        modelwindow.h \
        qfilefunctions.h \
        settings.h \
        utils/calibrateresult.h \
        utils/calibrator.h \
        utils/stereocalibrator.h

FORMS += \
        dialog.ui \
        mainwindow.ui \
        modelwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    icon.qrc
