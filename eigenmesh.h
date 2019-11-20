#ifndef EIGENMESH_H
#define EIGENMESH_H
#include <Eigen/Dense>

class EigenMesh
{
public:
    EigenMesh();
    Eigen::Matrix3Xf getPoints() const;
    void setPoints(const Eigen::Matrix3Xf &value);

    Eigen::Matrix3Xf getNormals() const;
    void setNormals(const Eigen::Matrix3Xf &value);

    Eigen::Matrix3Xi getFaces() const;
    void setFaces(const Eigen::Matrix3Xi &value);

    Eigen::Matrix4Xf getColors() const;
    void setColors(const Eigen::Matrix4Xf &value);
    float computeScale();
private:
    Eigen::Matrix3Xf points;
    Eigen::Matrix3Xf normals;
    Eigen::Matrix3Xi faces;
    Eigen::Matrix4Xf colors;
};

#endif // EIGENMESH_H
