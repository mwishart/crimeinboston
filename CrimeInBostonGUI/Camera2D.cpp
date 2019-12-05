#include "Camera2D.h"

Camera2D::Camera2D()
{
    m_view = QMatrix4x4();
    m_x = 0.0f;
    m_y = 0.0f;
    m_scale = 1.0f;
    updateViewMatrix();
}
Camera2D::~Camera2D()
{

}
void Camera2D::updateViewMatrix()
{
    m_view.setToIdentity();
    m_view.translate(-m_x, -m_y, 0.0f); //view matricies are always opposite signs relative to the scene coordinate system
    m_view.scale(m_scale, m_scale);
}

//accessors
const float* Camera2D::getViewData() const //used by OpenGL matrix stack
{
    return m_view.constData();
}
float Camera2D::getX() const
{
    return m_x;
}
float Camera2D::getY() const
{
    return m_y;
}
float Camera2D::getScale() const
{
    return m_scale;
}

QPointF Camera2D::mapPixelToScene(const QPoint &pixel) const
{
    QPointF v = QPointF(pixel.x(), pixel.y());
    QPointF sp = m_view.inverted().map(v);
    return sp;
}

//setters
void Camera2D::setX(float x)
{
    m_x = x;
    updateViewMatrix();
}
void Camera2D::setY(float y)
{
    m_y = y;
    updateViewMatrix();
}
void Camera2D::setPosition(float x, float y)
{
    m_x = x;
    m_y = y;
    updateViewMatrix();
}
void Camera2D::centerOn(float x, float y, int screen_width, int screen_height)
{
    m_x = x - static_cast<float>(screen_width) * 0.5f;
    m_y = y - static_cast<float>(screen_height) * 0.5f;
    updateViewMatrix();
}
void Camera2D::zoomIn()
{
    m_scale *= 2.0f;
    updateViewMatrix();
}
void Camera2D::zoomOut()
{
    m_scale *= 0.5f;
    updateViewMatrix();
}

//transformation functions
void Camera2D::moveX(float tx)
{
    m_x += tx;
    updateViewMatrix();
}
void Camera2D::moveY(float ty)
{
    m_y += ty;
    updateViewMatrix();
}
void Camera2D::move(float tx, float ty)
{
    m_x += tx;
    m_y += ty;
    updateViewMatrix();
}
