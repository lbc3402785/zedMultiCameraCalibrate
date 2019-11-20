#ifndef VIEWER_H
#define VIEWER_H
#include <QGLViewer/qglviewer.h>
#include "eigenmesh.h"
class Viewer : public QGLViewer
{
public:
    Viewer();
    ~Viewer();
    virtual void draw();
    virtual void init();
    bool stopDraw;
    enum showMode{POINTS,FACE,CONTOUR_LINE,FACE_CONTOUR_LINE};


    float getScale() const;
    void setScale(float value);

    EigenMesh srcMesh,dstMesh;
    showMode srcMode;
    showMode dstMode;
    Eigen::Vector3f srcDefaultColor;
    Eigen::Vector3f dstDefaultColor;
private:

    bool initialized;
    float scale;


    void drawMesh(EigenMesh& mesh,Eigen::Vector3f defaultColor,showMode mode);
    void setMaterialAndLight();

};

#endif // VIEWER_H
