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

void ModelWindow::inputSrcMesh(EigenMesh src)
{
    modelViewer->stopDraw=true;
    modelViewer->srcMesh=src;
    float curScale=src.computeScale();
    std::cout<<"curScale:"<<curScale<<std::endl<<std::flush;
    if(modelViewer->getScale()<curScale){
        modelViewer->setScale(curScale);
    }
    modelViewer->stopDraw=false;
    modelViewer->update();
}
