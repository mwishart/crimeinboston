#include "Utilities3D.h"
#include <QMatrix4x4>
#include <iostream>
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
QPoint Utilities3D::sceneXYZtoPixel(const QVector3D &position, const QMatrix4x4 &view_matrix, const QMatrix4x4 &projection, int screen_width, int screen_height)
{
    QPoint pixel(-1, -1);
    float x, y;

    QVector3D vp = view_matrix.map(position);
    QVector3D nd = projection.map(vp);

    //std::cout << "DEBUG: p=(" << nd.x() << ", " << nd.y() << ", " << nd.z() << ")" << std::endl;
    x = nd.x() * static_cast<float>(screen_width) * 0.5f;
    y = nd.y() * static_cast<float>(screen_height) * 0.5f;
    pixel = QPoint(static_cast<int>(x) + screen_width / 2, (static_cast<int>(y) - screen_height / 2) * -1);
    std::cout << "DEBUG: pixel=(" << pixel.x() << ", " << pixel.y() << ")" << std::endl;

    return pixel;
}
