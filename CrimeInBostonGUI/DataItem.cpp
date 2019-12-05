#include "DataItem.h"

//constructor
DataItem::DataItem()
{
    incident_number = 0;
    offence_code_group = offence_description = QString();
    district = District_Invalid;
    day_of_the_week = DOW_Invalid;
    ucr_part = ucr_street = location = QString();
    latitude = longitude = 0.0;
    offence_code = reporting_area = 0;
    date = QDate();
}
DataItem::DataItem(const DataItem &other)
{
    *this = other;
}
DataItem::~DataItem()
{

}

//operator overloads
DataItem& DataItem::operator=(const DataItem &other)
{
    incident_number = other.incident_number;
    offence_code_group = other.offence_code_group;
    offence_description = other.offence_description;
    district = other.district;
    day_of_the_week = other.day_of_the_week;
    ucr_part = other.ucr_part;
    ucr_street = other.ucr_street;
    location = other.location;
    latitude = other.latitude;
    longitude = other.longitude;
    offence_code = other.offence_code;
    reporting_area = other.reporting_area;
    date = other.date;
    return (*this);
}
bool DataItem::operator==(const DataItem &other) const
{
    return (incident_number == other.incident_number);
}
bool DataItem::operator!=(const DataItem &other) const
{
    return (incident_number != other.incident_number);
}
