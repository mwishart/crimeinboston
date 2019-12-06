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

    //std::cout << "DEBUG: position=(" << position.x() << ", " << position.y() << ", " << position.z() << ")" << std::endl;
    x = nd.x() * static_cast<float>(screen_width) * 0.5f;
    y = nd.y() * static_cast<float>(screen_height) * 0.5f;
    pixel = QPoint(static_cast<int>(x) + screen_width / 2, (static_cast<int>(y) - screen_height / 2) * -1);
    //std::cout << "DEBUG: pixel=(" << pixel.x() << ", " << pixel.y() << ")" << std::endl;

    return pixel;
}
QPolygon Utilities3D::scenePolyToPixelPoly(const QPolygonF &scene_polygon, const QMatrix4x4 &view_matrix, const QMatrix4x4 &projection, int screen_width, int screen_height)
{
    QPolygon output(scene_polygon.size());
    QVector3D xyz;

    for (int i = 0; i < output.size(); i++)
    {
        const QPointF& p = scene_polygon[i];
        xyz = QVector3D(static_cast<float>(p.x()), static_cast<float>(p.y()), 0.0f);
        output[i] = sceneXYZtoPixel(xyz, view_matrix, projection, screen_width, screen_height);
    }
    return output;
}
