#-------------------------------------------------
#
# Project created by QtCreator 2019-10-22T17:58:36
#
#-------------------------------------------------

QT       += core gui
QT += multimedia
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = captureThree
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11
OBJECTS_DIR=$${PWD}/build
OPENCV_INCLUDE_DIRS=D:\soft\opencv3\build\include
OPENCV_LIBRARY_DIRS=D:\soft\opencv3\build\x64\vc15\lib
INCLUDEPATH+="C:\Program Files (x86)\ZED SDK\include"
INCLUDEPATH+="C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v10.0\include"
LIBS+=-L"C:\Program Files (x86)\ZED SDK\lib" -lsl_zed64
LIBS+=-L"C:\Program Files (x86)\ZED SDK\lib" -lsl_input64
LIBS+=-L"C:\Program Files (x86)\ZED SDK\lib" -lsl_core64
LIBS+="C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v10.0\lib\x64\cuda.lib"
LIBS+="C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v10.0\lib\x64\cudart.lib"

INCLUDEPATH+=$$OPENCV_INCLUDE_DIRS
INCLUDEPATH+=$$(DLIB_ROOT)\include
LIBS+=-L$$OPENCV_LIBRARY_DIRS -lopencv_world344
LIBS+=-L$$OPENCV_LIBRARY_DIRS -lopencv_ximgproc344
LIBS+=-L$$OPENCV_LIBRARY_DIRS -lopencv_calib3d344
LIBS+=-L$$OPENCV_LIBRARY_DIRS -lopencv_features2d344
LIBS+=-L$$OPENCV_LIBRARY_DIRS -lopencv_flann344
LIBS+=-L$$OPENCV_LIBRARY_DIRS -lopencv_highgui344
LIBS+=-L$$OPENCV_LIBRARY_DIRS -lopencv_videoio344
LIBS+=-L$$OPENCV_LIBRARY_DIRS -lopencv_imgcodecs344
LIBS+=-L$$OPENCV_LIBRARY_DIRS -lopencv_core344
LIBS+=-L$$(DLIB_ROOT)\lib -ldlib19.16.99_release_64bit_msvc1900

SOURCES += \
        chessboardconfig.cpp \
        dialog.cpp \
        main.cpp \
        mainwindow.cpp \
        qfilefunctions.cpp \
        settings.cpp \
        utils/calibrateresult.cpp \
        utils/calibrator.cpp \
        utils/stereocalibrator.cpp

HEADERS += \
        chessboardconfig.h \
        dialog.h \
        mainwindow.h \
        qfilefunctions.h \
        settings.h \
        utils/calibrateresult.h \
        utils/calibrator.h \
        utils/stereocalibrator.h

FORMS += \
        dialog.ui \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    icon.qrc
