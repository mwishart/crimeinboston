#ifndef CRIMECOLLECTION_H
#define CRIMECOLLECTION_H

#include <QIODevice>
#include <QVector3D>

struct CrimeNode
{
    float x;
    float y;
    qint64 file_offset;
    unsigned char prediction;
};

class CrimeCollection
{
public:
    CrimeCollection();
    ~CrimeCollection();

    int crimeCount() const;
    const CrimeNode& crimeAt(int idx) const;
    QVector3D crimeXYZAt(int idx) const;

    void pushCrime(float x, float y, qint64 file_position, unsigned char prediction);
    void clearCrimes();

    QList<CrimeNode> m_crimes;
};

#endif // CRIMECOLLECTION_H
