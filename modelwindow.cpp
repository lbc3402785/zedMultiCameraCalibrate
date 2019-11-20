#include "modelwindow.h"
#include "ui_modelwindow.h"
ModelWindow::ModelWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ModelWindow)
{
    ui->setupUi(this);
    modelViewer=new Viewer();
    modelViewer->init();
    ui->modelLayout->addWidget(modelViewer);
}

ModelWindow::~ModelWindow()
{
    delete ui;
}
