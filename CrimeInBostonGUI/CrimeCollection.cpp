#include "CrimeCollection.h"

CrimeCollection::CrimeCollection()
{

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

void CrimeCollection::pushCrime(float x, float y, qint64 file_position, unsigned char prediction)
{
    CrimeNode cn;
    cn.x = x;
    cn.y = y;
    cn.file_offset = file_position;
    cn.prediction = prediction;
    m_crimes.append(cn);
}

void CrimeCollection::clearCrimes()
{
    m_crimes.clear();
}
