#include "viewer.h"
#include "GL/freeglut.h"
Viewer::Viewer()
{
    scale=1.0f;
    stopDraw=false;
    initialized=false;
    srcMode=showMode::CONTOUR_LINE;
    dstMode=showMode::POINTS;
    srcDefaultColor<<1,0,0;
    dstDefaultColor<<0,1,0;
    this->camera()->setType(qglviewer::Camera::ORTHOGRAPHIC);
    startAnimation();
}
Viewer::~Viewer()
{
    stopAnimation();
}



float Viewer::getScale() const
{
    return scale;
}

void Viewer::setScale(float value)
{
    scale = value;
}
void Viewer::drawMesh(EigenMesh &mesh,Eigen::Vector3f defaultColor,showMode mode)
{
    Eigen::Matrix3Xf points=mesh.getPoints();
    Eigen::Matrix3Xi faces=mesh.getFaces();
    Eigen::Matrix3Xf normals=mesh.getNormals();
    Eigen::Matrix4Xf colors=mesh.getColors();
    bool hasColor=colors.size()>0;
    bool hasNormal=normals.size()>0;
    bool hasFaces=faces.size()>0;
    if(points.size()==0)return;
    if(hasFaces&&mode!=showMode::POINTS){
        bool needDrawFace=(mode==FACE||mode==FACE_CONTOUR_LINE||mode==CONTOUR_LINE);
        if(needDrawFace){
            //std::cout<<11<<std::endl<<std::flush;
            if(mode==FACE||mode==FACE_CONTOUR_LINE){
                glEnable(GL_POLYGON_OFFSET_FILL);
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            }
            for(int i=0;i<faces.cols();++i){
                int i0=faces(0,i);
                int i1=faces(1,i);
                int i2=faces(2,i);
                Eigen::Vector4f colorAlpha(0,0,0,0);
                if(!hasColor){
                    memcpy(colorAlpha.data(),defaultColor.data(),sizeof(float)*3);
                }else{
                    colorAlpha=(colors.col(i0)/255+colors.col(i1)/255+colors.col(i2)/255)/3;
                }

                glColor4fv(colorAlpha.data());
                glBegin(GL_TRIANGLES);
                Eigen::Vector3f p0=points.col(i0)/scale;
                Eigen::Vector3f p1=points.col(i1)/scale;
                Eigen::Vector3f p2=points.col(i2)/scale;
                if(hasNormal){
                    Eigen::Vector3f n0=normals.col(i0);
                    glNormal3fv(n0.data());
                    Eigen::Vector3f n1=normals.col(i1);
                    glNormal3fv(n1.data());
                    Eigen::Vector3f n2=normals.col(i2);
                    glNormal3fv(n2.data());
                }
                glVertex3fv(p0.data());
                glVertex3fv(p1.data());
                glVertex3fv(p2.data());

                glEnd();
            }
        }
        bool need_draw_contour_line=(mode==CONTOUR_LINE||mode==FACE_CONTOUR_LINE);
        if(need_draw_contour_line){
            glEnable(GL_POLYGON_OFFSET_LINE);
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glColor3f(0,0,1);
            glLineWidth(1.0f);
            glPolygonOffset(1.0, 1.0);
           // std::cout<<33<<std::endl<<std::flush;
            for(int i=0;i<faces.cols();++i){
                int i0=faces(0,i);
                int i1=faces(1,i);
                int i2=faces(2,i);
                glBegin(GL_TRIANGLES);
                Eigen::Vector3f p0=points.col(i0)/scale;
                Eigen::Vector3f p1=points.col(i1)/scale;
                Eigen::Vector3f p2=points.col(i2)/scale;
                if(hasNormal){
                    Eigen::Vector3f n0=normals.col(i0);
                    glNormal3fv(n0.data());
                    Eigen::Vector3f n1=normals.col(i1);
                    glNormal3fv(n1.data());
                    Eigen::Vector3f n2=normals.col(i2);
                    glNormal3fv(n2.data());
                }
                glVertex3fv(p0.data());
                glVertex3fv(p1.data());
                glVertex3fv(p2.data());
                glEnd();
            }
        }
    }
    else{
        glDisable(GL_LIGHTING);
        glPointSize(2);
        glBegin(GL_POINTS);
        for(int i=0;i<points.cols();i++){
            Eigen::Vector3f p=points.col(i);
            p/=scale;
            if(hasColor){
                Eigen::Vector4f c =colors.col(i);
                c/=255.0;
                glColor4fv(c.data());
            }else{
                glColor3fv(defaultColor.data());
            }
            glVertex3fv(p.data());
        }
        glEnd();
    }
    glEnable(GL_LIGHTING);
}

void Viewer::setMaterialAndLight()
{
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ZERO);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glPointSize(1);

    glShadeModel(GL_SMOOTH);
    glEnable(GL_SMOOTH);
    float shine=100.0;
    float al=0.1f;
    float dl=0.8f;
    float sl=0.2f;

    float defaultAmbientColor[4] = {al,al,al,1.f};
    float defaultDiffuseColor[4] =  {dl,dl,dl,1.f};
    float defaultSpecularColor[4] = {sl,sl,sl,1.f};


    glMaterialfv(GL_FRONT, GL_AMBIENT,   defaultAmbientColor);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   defaultDiffuseColor);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  defaultSpecularColor);
    glMaterialfv(GL_FRONT, GL_SHININESS, &shine);

    glLightfv(GL_LIGHT0, GL_AMBIENT, defaultAmbientColor);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, defaultDiffuseColor);
    glLightfv(GL_LIGHT0, GL_SPECULAR, defaultSpecularColor);
    glLightfv(GL_LIGHT0, GL_SHININESS, &shine);

    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,GL_FALSE);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
}

void Viewer::draw(){
    if(stopDraw)return;
    glClear( GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT );
    preDraw();
    drawMesh(srcMesh,srcDefaultColor,srcMode);
    drawMesh(dstMesh,dstDefaultColor,dstMode);
    postDraw();
}
void Viewer::init(){
    if(!initialized){
        setMaterialAndLight();
        initialized=true;
    }
    if(!stopDraw){
        update();
    }
}
