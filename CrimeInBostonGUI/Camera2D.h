#ifndef CAMERA2D_H
#define CAMERA2D_H

#include <QMatrix4x4>

class Camera2D
{
public:
    Camera2D();
    ~Camera2D();

    //accessors
    const float* getViewData() const; //used by OpenGL matrix stack
    float getX() const;
    float getY() const;
    float getScale() const;
    QMatrix4x4 getViewMatrix() const;
    QPointF mapPixelToScene(const QPoint &pixel) const;

    //setters
    void setX(float x);
    void setY(float y);
    void setPosition(float x, float y);
    void centerOn(float x, float y, int screen_width, int screen_height);
    void zoomIn();
    void zoomOut();

    //transformation functions
    void moveX(float tx);
    void moveY(float ty);
    void move(float tx, float ty);

private:
    void updateViewMatrix();

private:
    QMatrix4x4 m_view;
    float m_x;
    float m_y;
    float m_scale;
};

#endif // CAMERA2D_H
