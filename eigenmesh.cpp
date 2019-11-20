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
