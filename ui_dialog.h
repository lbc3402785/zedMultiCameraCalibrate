/********************************************************************************
** Form generated from reading UI file 'dialog.ui'
**
** Created by: Qt User Interface Compiler version 5.13.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOG_H
#define UI_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Dialog
{
public:
    QTabWidget *tabWidget;
    QWidget *General;
    QGroupBox *groupBox;
    QLabel *wl;
    QLabel *numH;
    QLabel *numW;
    QLabel *hl;
    QSpinBox *boardW;
    QSpinBox *boardH;
    QSpinBox *dx;
    QSpinBox *dy;
    QGroupBox *groupBox_2;
    QLabel *dirInfo;
    QLineEdit *imageDir;
    QLabel *surffixInfo;
    QLineEdit *surffix;
    QLabel *outputInfo;
    QLineEdit *outputDir;
    QWidget *tab_2;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *reset;
    QSpacerItem *horizontalSpacer;
    QPushButton *cancel;
    QPushButton *save;

    void setupUi(QDialog *Dialog)
    {
        if (Dialog->objectName().isEmpty())
            Dialog->setObjectName(QString::fromUtf8("Dialog"));
        Dialog->resize(432, 502);
        tabWidget = new QTabWidget(Dialog);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setGeometry(QRect(10, 10, 411, 431));
        General = new QWidget();
        General->setObjectName(QString::fromUtf8("General"));
        groupBox = new QGroupBox(General);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(10, 20, 381, 191));
        wl = new QLabel(groupBox);
        wl->setObjectName(QString::fromUtf8("wl"));
        wl->setGeometry(QRect(22, 73, 174, 16));
        numH = new QLabel(groupBox);
        numH->setObjectName(QString::fromUtf8("numH"));
        numH->setGeometry(QRect(22, 47, 120, 16));
        numW = new QLabel(groupBox);
        numW->setObjectName(QString::fromUtf8("numW"));
        numW->setGeometry(QRect(22, 21, 120, 16));
        hl = new QLabel(groupBox);
        hl->setObjectName(QString::fromUtf8("hl"));
        hl->setGeometry(QRect(22, 99, 174, 16));
        boardW = new QSpinBox(groupBox);
        boardW->setObjectName(QString::fromUtf8("boardW"));
        boardW->setGeometry(QRect(202, 21, 40, 20));
        boardH = new QSpinBox(groupBox);
        boardH->setObjectName(QString::fromUtf8("boardH"));
        boardH->setGeometry(QRect(202, 47, 40, 20));
        dx = new QSpinBox(groupBox);
        dx->setObjectName(QString::fromUtf8("dx"));
        dx->setGeometry(QRect(202, 73, 40, 20));
        dy = new QSpinBox(groupBox);
        dy->setObjectName(QString::fromUtf8("dy"));
        dy->setGeometry(QRect(202, 99, 40, 20));
        groupBox_2 = new QGroupBox(General);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setGeometry(QRect(10, 219, 381, 141));
        dirInfo = new QLabel(groupBox_2);
        dirInfo->setObjectName(QString::fromUtf8("dirInfo"));
        dirInfo->setGeometry(QRect(31, 37, 84, 16));
        imageDir = new QLineEdit(groupBox_2);
        imageDir->setObjectName(QString::fromUtf8("imageDir"));
        imageDir->setGeometry(QRect(121, 37, 133, 20));
        surffixInfo = new QLabel(groupBox_2);
        surffixInfo->setObjectName(QString::fromUtf8("surffixInfo"));
        surffixInfo->setGeometry(QRect(31, 64, 60, 16));
        surffix = new QLineEdit(groupBox_2);
        surffix->setObjectName(QString::fromUtf8("surffix"));
        surffix->setGeometry(QRect(121, 64, 133, 20));
        outputInfo = new QLabel(groupBox_2);
        outputInfo->setObjectName(QString::fromUtf8("outputInfo"));
        outputInfo->setGeometry(QRect(31, 91, 60, 16));
        outputDir = new QLineEdit(groupBox_2);
        outputDir->setObjectName(QString::fromUtf8("outputDir"));
        outputDir->setGeometry(QRect(121, 91, 133, 20));
        tabWidget->addTab(General, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        tabWidget->addTab(tab_2, QString());
        layoutWidget = new QWidget(Dialog);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(10, 460, 381, 25));
        horizontalLayout_2 = new QHBoxLayout(layoutWidget);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        reset = new QPushButton(layoutWidget);
        reset->setObjectName(QString::fromUtf8("reset"));

        horizontalLayout_2->addWidget(reset);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        cancel = new QPushButton(layoutWidget);
        cancel->setObjectName(QString::fromUtf8("cancel"));

        horizontalLayout_2->addWidget(cancel);

        save = new QPushButton(layoutWidget);
        save->setObjectName(QString::fromUtf8("save"));

        horizontalLayout_2->addWidget(save);


        retranslateUi(Dialog);

        tabWidget->setCurrentIndex(0);
        save->setDefault(true);


        QMetaObject::connectSlotsByName(Dialog);
    } // setupUi

    void retranslateUi(QDialog *Dialog)
    {
        Dialog->setWindowTitle(QCoreApplication::translate("Dialog", "Dialog", nullptr));
        groupBox->setTitle(QCoreApplication::translate("Dialog", "chessboard", nullptr));
        wl->setText(QCoreApplication::translate("Dialog", "  \346\243\213\347\233\230\346\250\252\350\275\264\345\215\225\344\270\252\346\240\274\345\255\220\347\232\204\351\225\277\345\272\246(mm):", nullptr));
        numH->setText(QCoreApplication::translate("Dialog", "  \346\243\213\347\233\230\347\272\265\350\275\264\350\247\222\347\202\271\344\270\252\346\225\260\357\274\232", nullptr));
        numW->setText(QCoreApplication::translate("Dialog", "  \346\243\213\347\233\230\346\250\252\350\275\264\350\247\222\347\202\271\344\270\252\346\225\260\357\274\232", nullptr));
        hl->setText(QCoreApplication::translate("Dialog", "  \346\243\213\347\233\230\347\272\265\350\275\264\345\215\225\344\270\252\346\240\274\345\255\220\347\232\204\351\225\277\345\272\246(mm):", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("Dialog", "file", nullptr));
        dirInfo->setText(QCoreApplication::translate("Dialog", "\345\233\276\347\211\207\344\277\235\345\255\230\350\267\257\345\276\204\357\274\232", nullptr));
        surffixInfo->setText(QCoreApplication::translate("Dialog", "\345\233\276\347\211\207\346\240\274\345\274\217\357\274\232", nullptr));
        outputInfo->setText(QCoreApplication::translate("Dialog", "\350\276\223\345\207\272\350\267\257\345\276\204\357\274\232", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(General), QCoreApplication::translate("Dialog", "Tab 1", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QCoreApplication::translate("Dialog", "Tab 2", nullptr));
        reset->setText(QCoreApplication::translate("Dialog", "Reset", nullptr));
        cancel->setText(QCoreApplication::translate("Dialog", "Cancel", nullptr));
        save->setText(QCoreApplication::translate("Dialog", "Save", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Dialog: public Ui_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOG_H
