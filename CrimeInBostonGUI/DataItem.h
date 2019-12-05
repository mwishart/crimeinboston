#ifndef DATAITEM_H
#define DATAITEM_H

#include <QString>
#include <QDate>

enum DayOfWeek
{
    DOW_Invalid = -1,
    Monday = 0,
    Tuesday,
    Wednesday,
    Thursday,
    Friday,
    Saturday,
    Sunday
};
enum DistrictEnum
{
    District_Invalid = -1,
    District_A1 = 0,
    District_A7,
    District_A15,
    District_B2,
    District_B3,
    District_C6,
    District_C11,
    District_D4,
    District_D14,
    District_E5,
    District_E13,
    District_E18,
    District_Total
};

struct DataItem
{
public:
    DataItem();
    DataItem(const DataItem &other);
    ~DataItem();

    DataItem& operator=(const DataItem &other);
    bool operator==(const DataItem &other) const;
    bool operator!=(const DataItem &other) const;

public:
    int incident_number;
    QString offence_code_group;
    QString offence_description;
    DistrictEnum district;
    DayOfWeek day_of_the_week;
    QString ucr_part;
    QString ucr_street;
    QString location;
    double latitude;
    double longitude;
    int offence_code;
    int reporting_area;
    QDate date; //year and month are part of this datastructure so there is no need to include it in this struct
};

#endif // DATAITEM_H
