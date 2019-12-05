#include "BCView.h"
#include <QPaintEvent>
#include <QResizeEvent>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QPainter>
#include <iostream>
#include "CrimeCollection.h"
#include "Camera2D.h"
#include "Utilities2D.h"
#include "Utilities3D.h"

const float magic_x = static_cast<float>(-71.12753454091897 - District::ORIGIN_LON);
const float magic_y = 1.0f - static_cast<float>(42.26117620658846 - District::ORIGIN_LAT);

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
}
BCView::~BCView()
{
    for (int i = 0; i < District_Total; i++)
        delete m_districts[i];
    delete[] m_districts;
    delete m_camera;
}

int BCView::loadCrimeCSV(const QString &csv)
{
    int rc = 0;
    QStringList list;
    QFile file(csv);
    bool load_ok = file.open(QIODevice::ReadOnly | QIODevice::Text);
    DistrictEnum di;
    CrimeCollection* cc;
    double lat, lon;
    float x, y;
    if (load_ok)
    {
        //clear old crimes from districts
        for (int i = 0; i < District_Total; i++)
            m_districts[i]->crimeCollection()->clearCrimes();

        QString line = file.readLine(); //header
        while (!file.atEnd())
        {
            line = file.readLine();
            list = line.split(',');
            if (list.size() >= 17)
            {
                const QString& id = list.at(4);
                di = District::idToEnum(id);
                if (di != District_Invalid)
                {
                    const QString& latstr = list.at(14);
                    const QString& lonstr = list.at(15);
                    lat = latstr.toDouble();
                    lon = lonstr.toDouble();
                    x = static_cast<float>(lon - District::ORIGIN_LON);
                    y = 1.0f - static_cast<float>(lat - District::ORIGIN_LAT);
                    cc = m_districts[di]->crimeCollection();
                    cc->pushCrime(x, y, file.pos(), 0);
                }
            }
        }
        update();

        file.close();
        rc = 0;
    }
    else
    {
        std::cout << "ERROR: failed to open file \"" << csv.toStdString() << "\" for reading" << std::endl;
        rc = -1;
    }
    std::cout.flush();
    return rc;
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
    if (ev->button() == Qt::RightButton)
    {
        m_camera->setPosition(magic_x, magic_y);
        update();
    }
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
    if (m_panning)
    {
        const float m_camera_scale = m_camera->getScale();
        float delta_x, delta_y;

        delta_x = static_cast<float>(mp.x() - m_pan_point.x()) / m_camera_scale;
        delta_y = static_cast<float>(mp.y() - m_pan_point.y()) / m_camera_scale;
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
    //m_projection_2d.ortho(QRect(0, 0, width, height));
    //m_projection_2d.ortho(QRect(width / -2, height / -2, width / 2, height / 2));
    //m_projection_2d.ortho( 0, width, height, 0, -1, 1);
    m_projection_2d.ortho(-width/2.f, width/2.f, height/2.f, -height/2.f, -1, 1);
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


    //draw a bunch of fuckin quads to fix the black screen bullshit i see
    float s = 0.1f;
    f->glLineWidth(1.0f);
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

    //f->glTranslatef(width() / 2 / m_camera->getScale(), height() / 2 / m_camera->getScale(), 0.0f); //I give up


    for (int i = 0; i < District_Total; i++)
    {
        District* d = m_districts[i];
        if (d != nullptr)
            d->drawDistrictBorder();
    }
    f->glPointSize(1.0f);
    for (int i = 0; i < District_Total; i++)
    {
        District* d = m_districts[i];
        if (d != nullptr)
            d->drawCrimeData();
    }



    f->glBegin(GL_POINTS);
    f->glPointSize(16.0f);
    f->glColor3ub(100, 255, 100);
    f->glVertex2f(magic_x, magic_y);
    f->glEnd();

    //QPainter painter(this);

    //painter.drawImage(rect(), m_image);

    //painter.end();
}
