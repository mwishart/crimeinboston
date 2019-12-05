#ifndef DISTRICT_H
#define DISTRICT_H

#include <QString>
#include <QList>

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

    int loadData(const QString &d_filepath);

    struct LL
    {
        double lat;
        double lon;
        float x;
        float y;
    };

    void drawDistrict();

    static DistrictEnum idToEnum(const QString &name);
    static QString enumToId(DistrictEnum d_id);

private:
    QList<LL> m_positions;
    QString m_id;
    QString m_name;

public:
    static const double ORIGIN_LON;
    static const double ORIGIN_LAT;
};


#endif // DISTRICT_H
