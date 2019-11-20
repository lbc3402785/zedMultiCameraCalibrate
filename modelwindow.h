#ifndef MODELWINDOW_H
#define MODELWINDOW_H

#include <QMainWindow>
#include "graphics/viewer.h"
namespace Ui {
class ModelWindow;
}

class ModelWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ModelWindow(QWidget *parent = nullptr);
    ~ModelWindow();

private:
    Ui::ModelWindow *ui;
    Viewer* modelViewer;
};

#endif // MODELWINDOW_H
