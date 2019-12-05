#include "Utilities3D.h"
#include <cmath>

float Utilities3D::euclideanDistance(const QVector3D &a, const QVector3D &b)
{
    return sqrtf(euclideanDistanceSquared(a, b));
}
float Utilities3D::euclideanDistanceSquared(const QVector3D &a, const QVector3D &b)
{
    float dx, dy, dz;
    dx = b.x() - a.x();
    dy = b.y() - a.y();
    dz = b.z() - a.z();
    return (dx * dx) + (dy * dy) + (dz * dz);
}
