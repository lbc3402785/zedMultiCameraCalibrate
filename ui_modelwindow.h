/********************************************************************************
** Form generated from reading UI file 'modelwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.13.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MODELWINDOW_H
#define UI_MODELWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ModelWindow
{
public:
    QWidget *centralwidget;
    QGridLayout *formLayout;
    QGridLayout *gridLayout;
    QPushButton *loadSrcKey;
    QPushButton *loadDstKey;
    QVBoxLayout *modelLayout;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *ModelWindow)
    {
        if (ModelWindow->objectName().isEmpty())
            ModelWindow->setObjectName(QString::fromUtf8("ModelWindow"));
        ModelWindow->resize(1070, 706);
        centralwidget = new QWidget(ModelWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        formLayout = new QGridLayout(centralwidget);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        loadSrcKey = new QPushButton(centralwidget);
        loadSrcKey->setObjectName(QString::fromUtf8("loadSrcKey"));

        gridLayout->addWidget(loadSrcKey, 0, 0, 1, 1);

        loadDstKey = new QPushButton(centralwidget);
        loadDstKey->setObjectName(QString::fromUtf8("loadDstKey"));

        gridLayout->addWidget(loadDstKey, 0, 1, 1, 1);

        modelLayout = new QVBoxLayout();
        modelLayout->setObjectName(QString::fromUtf8("modelLayout"));

        gridLayout->addLayout(modelLayout, 2, 0, 1, 2);


        formLayout->addLayout(gridLayout, 0, 0, 1, 1);

        ModelWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(ModelWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1070, 23));
        ModelWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(ModelWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        ModelWindow->setStatusBar(statusbar);

        retranslateUi(ModelWindow);

        QMetaObject::connectSlotsByName(ModelWindow);
    } // setupUi

    void retranslateUi(QMainWindow *ModelWindow)
    {
        ModelWindow->setWindowTitle(QCoreApplication::translate("ModelWindow", "MainWindow", nullptr));
        loadSrcKey->setText(QCoreApplication::translate("ModelWindow", "load src key", nullptr));
        loadDstKey->setText(QCoreApplication::translate("ModelWindow", "load dst key", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ModelWindow: public Ui_ModelWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MODELWINDOW_H
