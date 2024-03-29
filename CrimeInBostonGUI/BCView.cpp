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
    m_selected_point.dis = District_Invalid;
    m_details = new CrimeNodeData();
}
BCView::~BCView()
{
    for (int i = 0; i < District_Total; i++)
        delete m_districts[i];
    delete[] m_districts;
    delete m_camera;
    delete m_details;
}

bool BCView::findClosestPointToPixel(SelectedPoint* sp, const QPoint &pixel, int max_range) const
{
    bool found = false;
    const int sw = width(), sh = height();
    const QRect viewport = rect();
    int closest_distance = (max_range * max_range), distance, count, i, j;
    const QMatrix4x4 view_matrix = m_camera->getViewMatrix();
    QPolygon poly;
    QPoint crime_pixel;
    for (i = 0; i < District_Total; i++)
    {
        CrimeCollection* cc = m_districts[i]->crimeCollection();
        poly = Utilities3D::scenePolyToPixelPoly(cc->getBounds(), view_matrix, m_projection_2d, sw, sh);
        if (!poly.containsPoint(pixel, Qt::OddEvenFill)) //skip regions that dont have the pixel
            continue;
        count = cc->crimeCount();
        for (j = 0; j < count; j++)
        {
            crime_pixel = Utilities3D::sceneXYZtoPixel(QVector3D(cc->crimeXYZAt(j)), view_matrix, m_projection_2d, sw, sh);
            if (viewport.contains(crime_pixel))
            {
                distance = Utilities2D::euclideanDistanceSquaredI(pixel, crime_pixel);
                if (distance < closest_distance)
                {
                    found = true;
                    closest_distance = distance;
                    sp->dis = static_cast<DistrictEnum>(i);
                    sp->index = j;
                    if (closest_distance == 0) //if it is zero we are done
                    {
                        i = District_Total;
                        break;
                    }
                }
            }
        }
    }

    if (!found)
    {
        sp->dis = District_Invalid;
        sp->index = -1;
    }

    return found;
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
        qint64 fp;
        //clear old crimes from districts
        for (int i = 0; i < District_Total; i++)
            m_districts[i]->crimeCollection()->clearCrimes();
        m_csv = csv;

        QString line = file.readLine(); //header
        while (!file.atEnd())
        {
            fp = file.pos();
            line = file.readLine();
            list = line.split(',');
            if (list.size() >= 17)
            {
                const QString& id = list.at(4);
                di = District::idToEnum(id);
                if (di != District_Invalid)
                {
                    unsigned char ucr_part = 0u;
                    const QString& latstr = list.at(14);
                    const QString& lonstr = list.at(15);
                    const QString ucr_str = list.at(12);
                    if (ucr_str.size() > 6)
                    {
                        switch(ucr_str[6].toLatin1())
                        {
                            case 'n': ucr_part = 1u; break;
                            case 'h': ucr_part = 2u; break;
                            case 'w': ucr_part = 3u; break;
                        }
                    }
                    lat = latstr.toDouble();
                    lon = lonstr.toDouble();
                    x = static_cast<float>(lon - District::ORIGIN_LON);
                    y = 1.0f - static_cast<float>(lat - District::ORIGIN_LAT);
                    cc = m_districts[di]->crimeCollection();
                    cc->pushCrime(x, y, fp, 0, ucr_part);
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
        findClosestPointToPixel(&m_selected_point, ev->pos(), 16);
        if (m_selected_point.dis != District_Invalid && m_selected_point.index >= 0)
        {
        qint64 fp = m_districts[m_selected_point.dis]->crimeCollection()->crimeAt(m_selected_point.index).file_offset;
        if (fp > 0u)
            m_details->readFromFile(m_csv, fp);
        }
        update();
    }
    else if (ev->button() == Qt::MiddleButton)
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
        Utilities3D::sceneXYZtoPixel(QVector3D(magic_x, magic_y, 0.0f), m_camera->getViewMatrix(), m_projection_2d, width(), height());

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

    f->glPointSize(4.0f);
    f->glBegin(GL_POINTS);
    f->glColor3ub(100, 255, 100);
    f->glVertex2f(magic_x, magic_y);
    f->glEnd();

    drawUI();
}

void BCView::drawUI()
{
    QPainter painter(this);

    if (m_selected_point.dis != District_Invalid && m_selected_point.index >= 0)
    {
        QString datestr;
        QPen pen;
        QBrush brush;
        QVector3D sp = m_districts[m_selected_point.dis]->crimeCollection()->crimeXYZAt(m_selected_point.index);
        QPoint pixel = Utilities3D::sceneXYZtoPixel(sp, m_camera->getViewMatrix(), m_projection_2d, width(), height());
        QRect textpos;
        int incr = 20;

        pen.setWidth(2);
        pen.setColor(QColor(255, 255, 255));
        brush.setStyle(Qt::NoBrush);

        painter.setPen(pen);
        painter.setBrush(brush);
        painter.drawEllipse(pixel, 8, 8);

        //draw details
        brush.setColor(QColor(0, 0, 0, 150));
        brush.setStyle(Qt::SolidPattern);
        pen.setColor(QColor(255, 255, 255));
        pen.setStyle(Qt::SolidLine);
        pen.setWidth(1);
        painter.setPen(pen);
        painter.setBrush(brush);
        painter.setFont(QFont("Verdana", 12));

        painter.drawRect(QRect(2, 2, 128, 8 * incr + 2));
        textpos = QRect(5, 5, 512, incr);

        painter.drawText(textpos, m_details->incident_number); textpos.translate(0, incr);
        painter.drawText(textpos, QString::number(m_details->lat, 'f', 2) + QString(", ") + QString::number(m_details->lon, 'f', 2)); textpos.translate(0, incr);
        painter.drawText(textpos, m_details->offence_code_group); textpos.translate(0, incr);
        painter.drawText(textpos, m_details->offence_description); textpos.translate(0, incr);
        painter.drawText(textpos, QString("District ") + m_details->district); textpos.translate(0, incr);
        painter.drawText(textpos, m_details->street); textpos.translate(0, incr);
        painter.drawText(textpos, m_details->day_of_the_week); textpos.translate(0, incr);

        datestr = QString::number(m_details->month);
        datestr.push_back('/');
        datestr.append(QString::number(m_details->year));
        datestr.push_back(' ');
        if (m_details->hour > 12)
            datestr.append(QString::number(m_details->hour - 12) + "pm");
        else
            datestr.append(QString::number(m_details->hour) + "am");
        painter.drawText(textpos, datestr);
    }

    painter.end();
}
