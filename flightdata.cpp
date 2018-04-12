#include "flightdata.h"
flightData* flightData::instance = 0;


flightData::flightData()
{

}

flightData* flightData::getInstance()
{
    if (instance == 0)
    {
        instance = new flightData();
    }

    return instance;
}

QString flightData::getRead_flightId() const
{
    return read_flightId;
}

void flightData::setRead_flightId(const QString &value)
{
    read_flightId = value;
}

QString flightData::getRead_code() const
{
    return read_code;
}

void flightData::setRead_code(const QString &value)
{
    read_code = value;
}

QString flightData::getRead_model() const
{
    return read_model;
}

void flightData::setRead_model(const QString &value)
{
    read_model = value;
}

QString flightData::getRead_tailNo() const
{
    return read_tailNo;
}

void flightData::setRead_tailNo(const QString &value)
{
    read_tailNo = value;
}

QString flightData::getRead_orgin() const
{
    return read_orgin;
}

void flightData::setRead_orgin(const QString &value)
{
    read_orgin = value;
}

QString flightData::getRead_destination() const
{
    return read_destination;
}

void flightData::setRead_destination(const QString &value)
{
    read_destination = value;
}

QString flightData::getRead_date(int i) const
{
    return read_date.at(i);
}

void flightData::setRead_date(const QString &value)
{
    read_date.append(value);
}

QString flightData::getRead_time(int i) const
{
    return read_time.at(i);
}

void flightData::setRead_time(const QString &value)
{
    read_time.append(value);
}

QString flightData::getRead_latitude(int i) const
{
    return read_latitude.at(i);
}

void flightData::setRead_latitude(const QString &value)
{
    read_latitude.append(value);
}

QString flightData::getRead_longitude(int i) const
{
    return read_longitude.at(i);
}

void flightData::setRead_longitude(const QString &value)
{
    read_longitude.append(value);
}

QString flightData::getRead_kmh(int i) const
{
    return read_kmh.at(i);
}

void flightData::setRead_kmh(const QString &value)
{
    read_kmh.append(value);
}

QString flightData::getRead_kts(int i) const
{
    return read_kts.at(i);
}

void flightData::setRead_kts(const QString &value)
{
    read_kts.append(value);
}

QString flightData::getRead_mph(int i) const
{
    return read_mph.at(i);
}

void flightData::setRead_mph(const QString &value)
{
    read_mph.append(value);
}

QString flightData::getRead_squwak(int i) const
{
    return read_squwak.at(i);
}

void flightData::setRead_squwak(const QString &value)
{
    read_squwak.append(value);
}

QString flightData::getRead_heading(int i) const
{
    return read_heading.at(i);
}

void flightData::setRead_heading(const QString &value)
{
    read_heading.append(value);
}

QString flightData::getRead_feet(int i) const
{
    return read_feet.at(i);
}

void flightData::setRead_feet(const QString &value)
{
    read_feet.append(value);
}

QString flightData::getRead_meters(int i) const
{
    return read_meters.at(i);
}

void flightData::setRead_meters(const QString &value)
{
    read_meters.append(value);
}

QString flightData::getRead_totalFeet(int i) const
{
    return read_totalFeet.at(i);
}

void flightData::setRead_totalFeet(const QString &value)
{
    read_totalFeet.append(value);
}


QString flightData::getRead_vvi(int i) const
{
    return read_vvi.at(i);
}

void flightData::setRead_vvi(const QString &value)
{
    read_vvi.append(value);
}

int flightData::size(){
    return read_longitude.size();
}

void flightData::clearAll(){
    read_date.clear();
    read_time.clear();
    read_latitude.clear();
    read_longitude.clear();
    read_kmh.clear();
    read_kts.clear();
    read_mph.clear();
    read_squwak.clear();
    read_heading.clear();
    read_feet.clear();
    read_meters.clear();
    read_totalFeet.clear();
    read_vvi.clear();

    read_flightId="";
    read_code="";
    read_model="";
    read_tailNo="";
    read_orgin="";
    read_destination="";
}

void flightData::removeLast(){
    read_latitude.removeLast();
    read_longitude.removeLast();
    read_meters.removeLast();
    read_time.removeLast();
}

void flightData::replaceRead_meters(int i, QString value){
    read_meters.replace(i,value);
}

