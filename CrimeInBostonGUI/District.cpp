#include "District.h"
#include "Utilities2D.h"
#include "Camera2D.h"
#include <QFile>
#include <iostream>
using namespace std;

const double District::ORIGIN_LON = -72.0;
const double District::ORIGIN_LAT = 42.0;

District::District(const QString &d_filepath)
{
    m_id = QString();
    m_name = QString("Unamed District");
    loadData(d_filepath);
}
District::District(DistrictEnum district_num)
{
    QString filename(":/districts/");
    m_id = QString();
    m_name = QString("Unamed District");
    filename.append(enumToId(district_num));
    filename.append(".txt");
    loadData(filename);
}

District::~District()
{

}

int District::loadData(const QString &d_filepath)
{
    int rc = 0, line = 1;
    QStringList list;
    QString ll_str;
    QFile file(d_filepath);
    bool load_ok = file.open(QIODevice::ReadOnly | QIODevice::Text);
    if (load_ok)
    {
        LL latlon;
        m_id = file.readLine(); line++;
        m_name = file.readLine(); line++;
        m_positions.clear();
        while (!file.atEnd())
        {
            ll_str = file.readLine();
            list = ll_str.split(',');
            if (list.size() == 2)
            {
                bool l_ok[2];
                latlon.lon = list[0].toDouble(&l_ok[0]);
                latlon.lat = list[1].toDouble(&l_ok[1]);
                latlon.x = static_cast<float>(latlon.lon - ORIGIN_LON);
                latlon.y = 1.0f - static_cast<float>(latlon.lat - ORIGIN_LAT);
                if (l_ok[0] && l_ok[1])
                    m_positions.append(latlon);
                else
                    cout << "ERROR: Failed to read lat lon in file for line " << line << endl;
            }
            line++;
        }

        if (m_positions.size() < 3)
            cout << "ERROR: Only got " << m_positions.size() << " from file \"" << d_filepath.toStdString() << "\"" << endl;

        file.close();
    }
    else
    {
        cout << "ERROR: failed to open file \"" << d_filepath.toStdString() << "\" for reading" << endl;
        rc = -1;
    }
    cout.flush();

    return rc;
}

void District::drawDistrict()
{
    QOpenGLFunctions_3_0* f = Utilities2D::getGLFunctions30();
    int size = m_positions.size();
    if (f != nullptr && size > 0)
    {
        f->glLineWidth(1.0f);
        f->glBegin(GL_LINE_LOOP);
        f->glColor3ub(255, 255, 100);
        for (int i = 0; i < size; i++)
        {
            const LL& xy = m_positions.at(i);
            f->glVertex2f(xy.x, xy.y);
        }

        f->glEnd();
    }
}

DistrictEnum District::idToEnum(const QString &name)
{
    DistrictEnum d = District_Invalid;
    if (name == "E18") d = District_E18;
    if (name == "E5") d = District_E5;
    return d;
}
QString District::enumToId(DistrictEnum d_id)
{
    QString id;
    switch(d_id)
    {
    case District_E18: id = "E18"; break;
    case District_E5: id = "E5"; break;
    default: id = "??"; break;
    }
    return id;
}
