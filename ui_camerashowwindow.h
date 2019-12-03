/********************************************************************************
** Form generated from reading UI file 'camerashowwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.13.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CAMERASHOWWINDOW_H
#define UI_CAMERASHOWWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CameraShowWindow
{
public:
    QAction *settings;
    QAction *exportExpression;
    QAction *actionsrc;
    QAction *actiondst;
    QAction *actionSrcPOINTS;
    QAction *actionSrcCONTOUR_LINE;
    QAction *actionDstPOINTS;
    QAction *actionDstCONTOUR_LINE;
    QWidget *centralWidget;
    QGridLayout *gridLayout_4;
    QGridLayout *gridLayout;
    QPushButton *next;
    QPushButton *last;
    QPushButton *loadDst;
    QPushButton *fittingExpression;
    QPushButton *loadExpressions;
    QPushButton *loadDstKey;
    QGraphicsView *graphicsView;
    QMenuBar *menuBar;
    QMenu *file;
    QMenu *tool;
    QMenu *menu;
    QMenu *menu_2;
    QMenu *menusrcMode;
    QMenu *menudstMode;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *CameraShowWindow)
    {
        if (CameraShowWindow->objectName().isEmpty())
            CameraShowWindow->setObjectName(QString::fromUtf8("CameraShowWindow"));
        CameraShowWindow->resize(1070, 706);
        settings = new QAction(CameraShowWindow);
        settings->setObjectName(QString::fromUtf8("settings"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/iconImage/img/settings.png"), QSize(), QIcon::Normal, QIcon::Off);
        settings->setIcon(icon);
        exportExpression = new QAction(CameraShowWindow);
        exportExpression->setObjectName(QString::fromUtf8("exportExpression"));
        actionsrc = new QAction(CameraShowWindow);
        actionsrc->setObjectName(QString::fromUtf8("actionsrc"));
        actionsrc->setCheckable(true);
        actiondst = new QAction(CameraShowWindow);
        actiondst->setObjectName(QString::fromUtf8("actiondst"));
        actiondst->setCheckable(true);
        actionSrcPOINTS = new QAction(CameraShowWindow);
        actionSrcPOINTS->setObjectName(QString::fromUtf8("actionSrcPOINTS"));
        actionSrcPOINTS->setCheckable(true);
        actionSrcCONTOUR_LINE = new QAction(CameraShowWindow);
        actionSrcCONTOUR_LINE->setObjectName(QString::fromUtf8("actionSrcCONTOUR_LINE"));
        actionSrcCONTOUR_LINE->setCheckable(true);
        actionDstPOINTS = new QAction(CameraShowWindow);
        actionDstPOINTS->setObjectName(QString::fromUtf8("actionDstPOINTS"));
        actionDstPOINTS->setCheckable(true);
        actionDstCONTOUR_LINE = new QAction(CameraShowWindow);
        actionDstCONTOUR_LINE->setObjectName(QString::fromUtf8("actionDstCONTOUR_LINE"));
        actionDstCONTOUR_LINE->setCheckable(true);
        centralWidget = new QWidget(CameraShowWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        gridLayout_4 = new QGridLayout(centralWidget);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        next = new QPushButton(centralWidget);
        next->setObjectName(QString::fromUtf8("next"));

        gridLayout->addWidget(next, 2, 2, 1, 1);

        last = new QPushButton(centralWidget);
        last->setObjectName(QString::fromUtf8("last"));

        gridLayout->addWidget(last, 2, 1, 1, 1);

        loadDst = new QPushButton(centralWidget);
        loadDst->setObjectName(QString::fromUtf8("loadDst"));

        gridLayout->addWidget(loadDst, 1, 0, 1, 1);

        fittingExpression = new QPushButton(centralWidget);
        fittingExpression->setObjectName(QString::fromUtf8("fittingExpression"));

        gridLayout->addWidget(fittingExpression, 2, 0, 1, 1);

        loadExpressions = new QPushButton(centralWidget);
        loadExpressions->setObjectName(QString::fromUtf8("loadExpressions"));

        gridLayout->addWidget(loadExpressions, 1, 1, 1, 1);

        loadDstKey = new QPushButton(centralWidget);
        loadDstKey->setObjectName(QString::fromUtf8("loadDstKey"));

        gridLayout->addWidget(loadDstKey, 1, 2, 1, 1);

        graphicsView = new QGraphicsView(centralWidget);
        graphicsView->setObjectName(QString::fromUtf8("graphicsView"));

        gridLayout->addWidget(graphicsView, 0, 0, 1, 3);


        gridLayout_4->addLayout(gridLayout, 0, 0, 1, 1);

        CameraShowWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(CameraShowWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1070, 23));
        file = new QMenu(menuBar);
        file->setObjectName(QString::fromUtf8("file"));
        tool = new QMenu(menuBar);
        tool->setObjectName(QString::fromUtf8("tool"));
        menu = new QMenu(tool);
        menu->setObjectName(QString::fromUtf8("menu"));
        menu_2 = new QMenu(menuBar);
        menu_2->setObjectName(QString::fromUtf8("menu_2"));
        menusrcMode = new QMenu(menu_2);
        menusrcMode->setObjectName(QString::fromUtf8("menusrcMode"));
        menudstMode = new QMenu(menu_2);
        menudstMode->setObjectName(QString::fromUtf8("menudstMode"));
        CameraShowWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(CameraShowWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        CameraShowWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(CameraShowWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        CameraShowWindow->setStatusBar(statusBar);

        menuBar->addAction(file->menuAction());
        menuBar->addAction(tool->menuAction());
        menuBar->addAction(menu_2->menuAction());
        file->addAction(settings);
        tool->addAction(exportExpression);
        tool->addAction(menu->menuAction());
        menu->addAction(actionsrc);
        menu->addAction(actiondst);
        menu_2->addAction(menusrcMode->menuAction());
        menu_2->addAction(menudstMode->menuAction());
        menusrcMode->addAction(actionSrcPOINTS);
        menusrcMode->addAction(actionSrcCONTOUR_LINE);
        menudstMode->addAction(actionDstPOINTS);
        menudstMode->addAction(actionDstCONTOUR_LINE);

        retranslateUi(CameraShowWindow);

        QMetaObject::connectSlotsByName(CameraShowWindow);
    } // setupUi

    void retranslateUi(QMainWindow *CameraShowWindow)
    {
        CameraShowWindow->setWindowTitle(QCoreApplication::translate("CameraShowWindow", "CameraShowWindow", nullptr));
        settings->setText(QCoreApplication::translate("CameraShowWindow", "\350\256\276\347\275\256", nullptr));
        exportExpression->setText(QCoreApplication::translate("CameraShowWindow", "\345\257\274\345\207\272", nullptr));
        actionsrc->setText(QCoreApplication::translate("CameraShowWindow", "src", nullptr));
        actiondst->setText(QCoreApplication::translate("CameraShowWindow", "dst", nullptr));
        actionSrcPOINTS->setText(QCoreApplication::translate("CameraShowWindow", "POINTS", nullptr));
        actionSrcCONTOUR_LINE->setText(QCoreApplication::translate("CameraShowWindow", "CONTOUR_LINE", nullptr));
        actionDstPOINTS->setText(QCoreApplication::translate("CameraShowWindow", "POINTS", nullptr));
        actionDstCONTOUR_LINE->setText(QCoreApplication::translate("CameraShowWindow", "CONTOUR_LINE", nullptr));
        next->setText(QCoreApplication::translate("CameraShowWindow", "next", nullptr));
        last->setText(QCoreApplication::translate("CameraShowWindow", "last", nullptr));
        loadDst->setText(QCoreApplication::translate("CameraShowWindow", "load dst", nullptr));
        fittingExpression->setText(QCoreApplication::translate("CameraShowWindow", "fit expression", nullptr));
        loadExpressions->setText(QCoreApplication::translate("CameraShowWindow", "load expression", nullptr));
        loadDstKey->setText(QCoreApplication::translate("CameraShowWindow", "load dst key", nullptr));
        file->setTitle(QCoreApplication::translate("CameraShowWindow", "\346\226\207\344\273\266", nullptr));
        tool->setTitle(QCoreApplication::translate("CameraShowWindow", "\345\267\245\345\205\267", nullptr));
        menu->setTitle(QCoreApplication::translate("CameraShowWindow", "\346\230\276\347\244\272", nullptr));
        menu_2->setTitle(QCoreApplication::translate("CameraShowWindow", "\346\250\241\345\274\217", nullptr));
        menusrcMode->setTitle(QCoreApplication::translate("CameraShowWindow", "srcMode", nullptr));
        menudstMode->setTitle(QCoreApplication::translate("CameraShowWindow", "dstMode", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CameraShowWindow: public Ui_CameraShowWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CAMERASHOWWINDOW_H
