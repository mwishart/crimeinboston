#ifndef CRIMECOLLECTION_H
#define CRIMECOLLECTION_H

#include <QIODevice>
#include <QVector3D>
#include <QPolygonF>

struct CrimeNode
{
    float x;
    float y;
    qint64 file_offset;
    unsigned char ucr_part;
    unsigned char prediction;
};
struct CrimeNodeData //more detailed data
{
    QString incident_number;
    double lat;
    double lon;
    QString offence_code_group;
    QString offence_description;
    QString district;
    int year;
    int month;
    QString day_of_the_week;
    int hour;
    QString street;

    int readFromFile(const QString &csv_file, qint64 file_position);
};

class CrimeCollection
{
public:
    CrimeCollection();
    ~CrimeCollection();

    int crimeCount() const;
    const CrimeNode& crimeAt(int idx) const;
    QVector3D crimeXYZAt(int idx) const;
    QPolygonF getBounds() const;

    void pushCrime(float x, float y, qint64 file_position, unsigned char prediction, unsigned char ucr_part);
    void clearCrimes();

    QList<CrimeNode> m_crimes;
    float m_left;
    float m_right;
    float m_top;
    float m_bottom;
};

#endif // CRIMECOLLECTION_H
