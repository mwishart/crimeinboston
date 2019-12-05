#ifndef BCVIEW_H
#define BCVIEW_H

#include <QOpenGLWidget>
#include <QMatrix4x4>
#include <QImage>
#include "District.h"

class Camera2D;

class BCView : public QOpenGLWidget
{
    Q_OBJECT
public:
    explicit BCView(QWidget *parent = nullptr);
    ~BCView();

protected:
    void initializeGL();

    void mouseReleaseEvent(QMouseEvent* ev);
    void mousePressEvent(QMouseEvent* ev);
    void mouseMoveEvent(QMouseEvent* ev);
    void wheelEvent(QWheelEvent* ev);
    void leaveEvent(QEvent* ev);

    void resizeGL(int width, int height);
    void paintGL();

signals:

public slots:

private:
    QImage m_image;
    QMatrix4x4 m_projection_2d;
    Camera2D* m_camera;
    District** m_districts;
    QPoint m_pan_point;
    bool m_panning;
};

#endif // BCVIEW_H
