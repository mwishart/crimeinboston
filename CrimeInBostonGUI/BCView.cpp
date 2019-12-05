#include "BCView.h"
#include <QPaintEvent>
#include <QResizeEvent>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QPainter>
#include <iostream>
#include "Camera2D.h"
#include "Utilities2D.h"
#include "Utilities3D.h"

BCView::BCView(QWidget *parent) : QOpenGLWidget(parent)
{
    m_image = QImage(":/images/boston_police_districts_f55.png");
    m_projection_2d.setToIdentity();
    m_districts = new District*[District_Total];
    for (int i = 0; i < District_Total; i++)
    {
        DistrictEnum d = static_cast<DistrictEnum>(i);
        m_districts[i] = new District(d);
    }
    m_panning = false;
    m_pan_point = QPoint();
    m_camera = new Camera2D();
    //m_camera->centerOn(0.0f, 0.0f, width(), height());
}
BCView::~BCView()
{
    for (int i = 0; i < District_Total; i++)
        delete m_districts[i];
    delete[] m_districts;
    delete m_camera;
}

void BCView::initializeGL()
{
    Utilities2D::setContext(context());
    QOpenGLFunctions_3_0* f = Utilities2D::getGLFunctions30();
    if (f == nullptr)
    {
        std::cout << "Failed to set OpenGL context. This whole display is worthless now." << std::endl;
        std::cout.flush();
        return;
    }

    f->glClearColor(0, 0, 0, 0);
    f->glDisable(GL_DEPTH_TEST);
    update();
}

void BCView::mouseReleaseEvent(QMouseEvent* ev)
{
    if (ev->button() == Qt::LeftButton)
        m_panning = false;
    ev->accept();
}
void BCView::mousePressEvent(QMouseEvent* ev)
{
    if (ev->button() == Qt::LeftButton)
    {
        m_panning = true;
        m_pan_point = ev->pos();
    }
    ev->accept();
}
void BCView::mouseMoveEvent(QMouseEvent* ev)
{
    const QPoint mp = ev->pos();
    int delta_x, delta_y;

    if (m_panning)
    {
        delta_x = mp.x() - m_pan_point.x();
        delta_y = mp.y() - m_pan_point.y();
        m_camera->move(delta_x, delta_y);
        m_pan_point = mp;
        update();
    }
    ev->accept();
}
void BCView::wheelEvent(QWheelEvent* ev)
{
    if (ev->delta() < 0)
    {
        m_camera->zoomOut();
        //m_camera->centerOn(m_camera->getX(), m_camera->getY(), width(), height());
        update();
    }
    else if (ev->delta() > 0)
    {
        m_camera->zoomIn();
        //m_camera->centerOn(m_camera->getX(), m_camera->getY(), width(), height());
        update();
    }
    ev->accept();
}

void BCView::leaveEvent(QEvent *ev)
{
    ev->accept();
    m_panning = false;
}

void BCView::resizeGL(int width, int height)
{
    //2d orthographic projection
    m_projection_2d.setToIdentity();
    m_projection_2d.ortho(QRect(0, 0, width, height));
    update();
}

void BCView::paintGL()
{
    QOpenGLFunctions_3_0* f = Utilities2D::getGLFunctions30();
    if (f == nullptr)
        return;

    f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    f->glMatrixMode(GL_PROJECTION);
    f->glLoadMatrixf(m_projection_2d.constData());
    f->glMatrixMode(GL_MODELVIEW);
    f->glLoadIdentity();
    f->glLoadMatrixf(m_camera->getViewData());
    //f->glTranslatef(width() / 2 / m_camera->getScale(), height() / 2 / m_camera->getScale(), 0.0f); //I give up
    f->glPointSize(3.0f);

    for (int i = 0; i < District_Total; i++)
    {
        District* d = m_districts[i];
        if (d != nullptr)
            d->drawDistrict();
    }

    //draw a bunch of fuckin quads to fix the black screen bullshit i see
    float s = 0.1f;
    for (int i = 0; i < 24; i++)
    {
    f->glBegin(GL_LINE_LOOP);
    f->glColor3ub(150, 150, 150);
    f->glVertex2f(-s, -s);
    f->glVertex2f(-s, s);
    f->glVertex2f(s, s);
    f->glVertex2f(s, -s);
    f->glEnd();
    s *= 10.0f;
    }

    //QPainter painter(this);

    //painter.drawImage(rect(), m_image);

    //painter.end();
}
