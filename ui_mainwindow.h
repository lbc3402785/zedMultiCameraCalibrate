/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.13.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *settings;
    QAction *calibrateStereo;
    QAction *clearFile;
    QAction *action_Image;
    QAction *action_Cloud;
    QAction *action_Depth;
    QAction *mergeCloud;
    QWidget *centralWidget;
    QGridLayout *gridLayout_4;
    QGridLayout *gridLayout;
    QComboBox *leftId;
    QPushButton *openLeft;
    QPushButton *closeLeft;
    QLCDNumber *leftLcdNumber;
    QLabel *left;
    QPushButton *saveLeft;
    QGraphicsView *leftView;
    QGridLayout *gridLayout_2;
    QLabel *middle;
    QComboBox *middleId;
    QGraphicsView *middleView;
    QPushButton *openMiddle;
    QPushButton *closeMiddle;
    QPushButton *saveMiddle;
    QLCDNumber *middleLcdNumber;
    QGridLayout *gridLayout_3;
    QLabel *right;
    QComboBox *rightId;
    QGraphicsView *rightView;
    QPushButton *openRight;
    QPushButton *closeRight;
    QPushButton *saveRight;
    QLCDNumber *rightLcdNumber;
    QHBoxLayout *horizontalLayout;
    QPushButton *open;
    QPushButton *close;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *left2middle;
    QPushButton *right2middle;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *check;
    QPushButton *save;
    QMenuBar *menuBar;
    QMenu *file;
    QMenu *tool;
    QMenu *menu;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(2508, 706);
        settings = new QAction(MainWindow);
        settings->setObjectName(QString::fromUtf8("settings"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icon/img/settings.png"), QSize(), QIcon::Normal, QIcon::Off);
        settings->setIcon(icon);
        calibrateStereo = new QAction(MainWindow);
        calibrateStereo->setObjectName(QString::fromUtf8("calibrateStereo"));
        clearFile = new QAction(MainWindow);
        clearFile->setObjectName(QString::fromUtf8("clearFile"));
        action_Image = new QAction(MainWindow);
        action_Image->setObjectName(QString::fromUtf8("action_Image"));
        action_Image->setCheckable(true);
        action_Cloud = new QAction(MainWindow);
        action_Cloud->setObjectName(QString::fromUtf8("action_Cloud"));
        action_Cloud->setCheckable(true);
        action_Depth = new QAction(MainWindow);
        action_Depth->setObjectName(QString::fromUtf8("action_Depth"));
        action_Depth->setCheckable(true);
        mergeCloud = new QAction(MainWindow);
        mergeCloud->setObjectName(QString::fromUtf8("mergeCloud"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        gridLayout_4 = new QGridLayout(centralWidget);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        leftId = new QComboBox(centralWidget);
        leftId->setObjectName(QString::fromUtf8("leftId"));

        gridLayout->addWidget(leftId, 0, 2, 1, 1);

        openLeft = new QPushButton(centralWidget);
        openLeft->setObjectName(QString::fromUtf8("openLeft"));

        gridLayout->addWidget(openLeft, 2, 0, 1, 1);

        closeLeft = new QPushButton(centralWidget);
        closeLeft->setObjectName(QString::fromUtf8("closeLeft"));

        gridLayout->addWidget(closeLeft, 2, 1, 1, 2);

        leftLcdNumber = new QLCDNumber(centralWidget);
        leftLcdNumber->setObjectName(QString::fromUtf8("leftLcdNumber"));

        gridLayout->addWidget(leftLcdNumber, 3, 1, 1, 2);

        left = new QLabel(centralWidget);
        left->setObjectName(QString::fromUtf8("left"));
        left->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(left, 0, 0, 1, 2);

        saveLeft = new QPushButton(centralWidget);
        saveLeft->setObjectName(QString::fromUtf8("saveLeft"));

        gridLayout->addWidget(saveLeft, 3, 0, 1, 1);

        leftView = new QGraphicsView(centralWidget);
        leftView->setObjectName(QString::fromUtf8("leftView"));

        gridLayout->addWidget(leftView, 1, 0, 1, 3);


        gridLayout_4->addLayout(gridLayout, 0, 0, 1, 1);

        gridLayout_2 = new QGridLayout();
        gridLayout_2->setSpacing(6);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        middle = new QLabel(centralWidget);
        middle->setObjectName(QString::fromUtf8("middle"));
        middle->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(middle, 0, 0, 1, 1);

        middleId = new QComboBox(centralWidget);
        middleId->setObjectName(QString::fromUtf8("middleId"));

        gridLayout_2->addWidget(middleId, 0, 1, 1, 1);

        middleView = new QGraphicsView(centralWidget);
        middleView->setObjectName(QString::fromUtf8("middleView"));

        gridLayout_2->addWidget(middleView, 1, 0, 1, 2);

        openMiddle = new QPushButton(centralWidget);
        openMiddle->setObjectName(QString::fromUtf8("openMiddle"));

        gridLayout_2->addWidget(openMiddle, 2, 0, 1, 1);

        closeMiddle = new QPushButton(centralWidget);
        closeMiddle->setObjectName(QString::fromUtf8("closeMiddle"));

        gridLayout_2->addWidget(closeMiddle, 2, 1, 1, 1);

        saveMiddle = new QPushButton(centralWidget);
        saveMiddle->setObjectName(QString::fromUtf8("saveMiddle"));

        gridLayout_2->addWidget(saveMiddle, 3, 0, 1, 1);

        middleLcdNumber = new QLCDNumber(centralWidget);
        middleLcdNumber->setObjectName(QString::fromUtf8("middleLcdNumber"));

        gridLayout_2->addWidget(middleLcdNumber, 3, 1, 1, 1);


        gridLayout_4->addLayout(gridLayout_2, 0, 1, 1, 1);

        gridLayout_3 = new QGridLayout();
        gridLayout_3->setSpacing(6);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        right = new QLabel(centralWidget);
        right->setObjectName(QString::fromUtf8("right"));
        right->setAlignment(Qt::AlignCenter);

        gridLayout_3->addWidget(right, 0, 0, 1, 2);

        rightId = new QComboBox(centralWidget);
        rightId->setObjectName(QString::fromUtf8("rightId"));

        gridLayout_3->addWidget(rightId, 0, 2, 1, 1);

        rightView = new QGraphicsView(centralWidget);
        rightView->setObjectName(QString::fromUtf8("rightView"));

        gridLayout_3->addWidget(rightView, 1, 0, 1, 3);

        openRight = new QPushButton(centralWidget);
        openRight->setObjectName(QString::fromUtf8("openRight"));

        gridLayout_3->addWidget(openRight, 2, 0, 1, 1);

        closeRight = new QPushButton(centralWidget);
        closeRight->setObjectName(QString::fromUtf8("closeRight"));

        gridLayout_3->addWidget(closeRight, 2, 1, 1, 2);

        saveRight = new QPushButton(centralWidget);
        saveRight->setObjectName(QString::fromUtf8("saveRight"));

        gridLayout_3->addWidget(saveRight, 3, 0, 1, 1);

        rightLcdNumber = new QLCDNumber(centralWidget);
        rightLcdNumber->setObjectName(QString::fromUtf8("rightLcdNumber"));

        gridLayout_3->addWidget(rightLcdNumber, 3, 1, 1, 2);


        gridLayout_4->addLayout(gridLayout_3, 0, 2, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        open = new QPushButton(centralWidget);
        open->setObjectName(QString::fromUtf8("open"));

        horizontalLayout->addWidget(open);

        close = new QPushButton(centralWidget);
        close->setObjectName(QString::fromUtf8("close"));

        horizontalLayout->addWidget(close);


        gridLayout_4->addLayout(horizontalLayout, 1, 0, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        left2middle = new QPushButton(centralWidget);
        left2middle->setObjectName(QString::fromUtf8("left2middle"));

        horizontalLayout_2->addWidget(left2middle);

        right2middle = new QPushButton(centralWidget);
        right2middle->setObjectName(QString::fromUtf8("right2middle"));

        horizontalLayout_2->addWidget(right2middle);


        gridLayout_4->addLayout(horizontalLayout_2, 1, 1, 1, 1);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        check = new QPushButton(centralWidget);
        check->setObjectName(QString::fromUtf8("check"));

        horizontalLayout_3->addWidget(check);

        save = new QPushButton(centralWidget);
        save->setObjectName(QString::fromUtf8("save"));

        horizontalLayout_3->addWidget(save);


        gridLayout_4->addLayout(horizontalLayout_3, 1, 2, 1, 1);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 2508, 23));
        file = new QMenu(menuBar);
        file->setObjectName(QString::fromUtf8("file"));
        tool = new QMenu(menuBar);
        tool->setObjectName(QString::fromUtf8("tool"));
        menu = new QMenu(tool);
        menu->setObjectName(QString::fromUtf8("menu"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(file->menuAction());
        menuBar->addAction(tool->menuAction());
        file->addAction(settings);
        tool->addAction(clearFile);
        tool->addAction(menu->menuAction());
        tool->addAction(mergeCloud);
        menu->addAction(action_Image);
        menu->addAction(action_Cloud);
        menu->addAction(action_Depth);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        settings->setText(QCoreApplication::translate("MainWindow", "\350\256\276\347\275\256", nullptr));
        calibrateStereo->setText(QCoreApplication::translate("MainWindow", "\345\217\214\347\233\256\346\240\207\345\256\232", nullptr));
        clearFile->setText(QCoreApplication::translate("MainWindow", "\346\270\205\347\251\272\346\226\207\344\273\266", nullptr));
        action_Image->setText(QCoreApplication::translate("MainWindow", "\345\233\276\347\211\207", nullptr));
        action_Cloud->setText(QCoreApplication::translate("MainWindow", "\347\202\271\344\272\221", nullptr));
        action_Depth->setText(QCoreApplication::translate("MainWindow", "\346\267\261\345\272\246\345\233\276", nullptr));
        mergeCloud->setText(QCoreApplication::translate("MainWindow", "\345\220\210\345\271\266\347\202\271\344\272\221", nullptr));
        openLeft->setText(QCoreApplication::translate("MainWindow", "open left", nullptr));
        closeLeft->setText(QCoreApplication::translate("MainWindow", "close left", nullptr));
        left->setText(QCoreApplication::translate("MainWindow", "left  id:", nullptr));
        saveLeft->setText(QCoreApplication::translate("MainWindow", "save left", nullptr));
        middle->setText(QCoreApplication::translate("MainWindow", "middle id:", nullptr));
        openMiddle->setText(QCoreApplication::translate("MainWindow", "open middle", nullptr));
        closeMiddle->setText(QCoreApplication::translate("MainWindow", "close middle", nullptr));
        saveMiddle->setText(QCoreApplication::translate("MainWindow", "save middle", nullptr));
        right->setText(QCoreApplication::translate("MainWindow", "right  id:", nullptr));
        openRight->setText(QCoreApplication::translate("MainWindow", "open right", nullptr));
        closeRight->setText(QCoreApplication::translate("MainWindow", "close right", nullptr));
        saveRight->setText(QCoreApplication::translate("MainWindow", "save right", nullptr));
        open->setText(QCoreApplication::translate("MainWindow", "open all", nullptr));
        close->setText(QCoreApplication::translate("MainWindow", "close all", nullptr));
        left2middle->setText(QCoreApplication::translate("MainWindow", "left-middle", nullptr));
        right2middle->setText(QCoreApplication::translate("MainWindow", "right-middle", nullptr));
        check->setText(QCoreApplication::translate("MainWindow", "find corner", nullptr));
        save->setText(QCoreApplication::translate("MainWindow", "save all", nullptr));
        file->setTitle(QCoreApplication::translate("MainWindow", "\346\226\207\344\273\266", nullptr));
        tool->setTitle(QCoreApplication::translate("MainWindow", "\345\267\245\345\205\267", nullptr));
        menu->setTitle(QCoreApplication::translate("MainWindow", "\344\277\235\345\255\230", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
