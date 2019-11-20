#include "eigenmesh.h"

EigenMesh::EigenMesh()
{

}

Eigen::Matrix3Xf EigenMesh::getPoints() const
{
    return points;
}

void EigenMesh::setPoints(const Eigen::Matrix3Xf &value)
{
    points = value;
}

Eigen::Matrix3Xf EigenMesh::getNormals() const
{
    return normals;
}

void EigenMesh::setNormals(const Eigen::Matrix3Xf &value)
{
    normals = value;
}

Eigen::Matrix3Xi EigenMesh::getFaces() const
{
    return faces;
}

void EigenMesh::setFaces(const Eigen::Matrix3Xi &value)
{
    faces = value;
}

Eigen::Matrix4Xf EigenMesh::getColors() const
{
    return colors;
}

void EigenMesh::setColors(const Eigen::Matrix4Xf &value)
{
    colors = value;
}

float EigenMesh::computeScale()
{
    int vnum=points.cols();
    float maxX=1e-5;
    float maxY=1e-5;
    float maxZ=1e-5;
    float minX=1e5;
    float minY=1e5;
    float minZ=1e5;
    for (int i=0;i<vnum;i++) {
        float x = points(0,i),y = points(1,i),z = points(2,i);
        maxX = std::fmax(x ,maxX ); maxY = std::fmax(y ,maxY ) ; maxZ = std::fmax(z ,maxZ );
        minX = std::fmin(x ,minX ); minY = std::fmin(y ,minY ) ; minZ = std::fmin(z ,minZ );
    }
    float xlen,ylen,zlen;
    xlen = fabs(maxX-minX);
    ylen = fabs(maxY-minY);
    zlen = fabs(maxZ-minZ);
    Eigen::Vector3f meshScale(xlen,ylen,zlen);
    return meshScale.maxCoeff();
}
