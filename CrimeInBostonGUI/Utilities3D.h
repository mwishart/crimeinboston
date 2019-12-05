#ifndef UTILITIES3D_H
#define UTILITIES3D_H

#include <QVector3D>

class Utilities3D
{
public:
    static float euclideanDistance(const QVector3D &a, const QVector3D &b);
    static float euclideanDistanceSquared(const QVector3D &a, const QVector3D &b);
    static QPoint sceneXYZtoPixel(const QVector3D &position, const QMatrix4x4 &view_matrix, const QMatrix4x4 &projection, int screen_width, int screen_height);
};

#endif // UTILITIES3D_H
