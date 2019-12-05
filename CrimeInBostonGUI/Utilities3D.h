#ifndef UTILITIES3D_H
#define UTILITIES3D_H

#include <QVector3D>

class Utilities3D
{
public:
    static float euclideanDistance(const QVector3D &a, const QVector3D &b);
    static float euclideanDistanceSquared(const QVector3D &a, const QVector3D &b);
};

#endif // UTILITIES3D_H
