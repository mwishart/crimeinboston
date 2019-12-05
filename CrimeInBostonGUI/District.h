#ifndef DISTRICT_H
#define DISTRICT_H

#include <QString>
#include <QList>

class CrimeCollection;

enum DistrictEnum
{
    District_Invalid = -1,
    District_E18,
    District_E5,
    District_Total
};

class District
{
public:
    District(const QString &d_filepath);
    District(DistrictEnum district_num);
    ~District();

    CrimeCollection* crimeCollection();
    int loadData(const QString &d_filepath);

    struct LL
    {
        double lat;
        double lon;
        float x;
        float y;
    };

    void drawDistrictBorder();
    void drawCrimeData();

    static DistrictEnum idToEnum(const QString &name);
    static QString enumToId(DistrictEnum d_id);

private:
    QList<LL> m_positions;
    CrimeCollection* m_crimes;
    QString m_id;
    QString m_name;

public:
    static const double ORIGIN_LON;
    static const double ORIGIN_LAT;
};


#endif // DISTRICT_H
