#include "mainwindow.h"
#include <QApplication>
#include "test/test.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
//    Test::testMulCamThread();
    return a.exec();
}
