#include "CrimeCollection.h"
#include <QFile>

CrimeCollection::CrimeCollection()
{
    m_crimes = QList<CrimeNode>();
    m_left = m_right = m_bottom = m_top = 0.0f;
}
CrimeCollection::~CrimeCollection()
{
    m_crimes.clear();
}

int CrimeCollection::crimeCount() const
{
    return m_crimes.size();
}
const CrimeNode &CrimeCollection::crimeAt(int idx) const
{
    return m_crimes.at(idx);
}
QVector3D CrimeCollection::crimeXYZAt(int idx) const
{
    const CrimeNode &cn = m_crimes.at(idx);
    return QVector3D(cn.x, cn.y, 0.0f);
}
QPolygonF CrimeCollection::getBounds() const
{
    QPolygonF poly(4);
    poly[0] = QPointF(static_cast<qreal>(m_left), static_cast<qreal>(m_bottom));
    poly[1] = QPointF(static_cast<qreal>(m_left), static_cast<qreal>(m_top));
    poly[2] = QPointF(static_cast<qreal>(m_right), static_cast<qreal>(m_top));
    poly[3] = QPointF(static_cast<qreal>(m_right), static_cast<qreal>(m_bottom));
    return poly;
}

void CrimeCollection::pushCrime(float x, float y, qint64 file_position, unsigned char prediction, unsigned char ucr_part)
{
    CrimeNode cn;
    cn.x = x;
    cn.y = y;
    cn.file_offset = file_position;
    cn.prediction = prediction;
    cn.ucr_part = ucr_part;
    m_crimes.append(cn);

    if (m_crimes.size() == 1)
    {
        m_left = m_right = x;
        m_top = m_bottom = y;
    }
    else
    {
        if (x < m_left) m_left = x;
        if (x > m_right) m_right = x;
        if (y < m_bottom) m_bottom = y;
        if (y > m_top) m_top = y;
    }
}

void CrimeCollection::clearCrimes()
{
    m_crimes.clear();
    m_left = m_right = m_bottom = m_top = 0.0f;
}

int CrimeNodeData::readFromFile(const QString &csv_file, qint64 file_position)
{
    int rc = 0;
    QStringList list;
    QFile file(csv_file);
    bool load_ok = file.open(QIODevice::ReadOnly | QIODevice::Text);
    if (load_ok)
    {
        QString line;

        file.seek(file_position);
        line = file.readLine();
        list = line.split(',');
        if (list.size() >= 17)
        {
            incident_number = list.at(0);
            lat = list.at(14).toDouble();
            lon = list.at(15).toDouble();
            offence_code_group = list.at(2);
            offence_description = list.at(3);
            district = list.at(4);
            year = list.at(8).toInt();
            month = list.at(9).toInt();
            day_of_the_week = list.at(10);
            hour = list.at(11).toInt();
            street = list.at(13);
        }
        file.close();
    }
    else
        rc = -1;
    return rc;
}
