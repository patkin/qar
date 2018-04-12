#ifndef FLIGHTDATA_H
#define FLIGHTDATA_H
#include <QString>
#include <QStringList>


class flightData
{
public:
    static flightData* getInstance( );
           ~flightData( );

    QString getRead_flightId() const;
    void setRead_flightId(const QString &value);

    QString getRead_code() const;
    void setRead_code(const QString &value);

    QString getRead_model() const;
    void setRead_model(const QString &value);

    QString getRead_tailNo() const;
    void setRead_tailNo(const QString &value);

    QString getRead_orgin() const;
    void setRead_orgin(const QString &value);

    QString getRead_destination() const;
    void setRead_destination(const QString &value);

    QString getRead_date(int i) const;
    void setRead_date(const QString &value);

    QString getRead_time(int i) const;
    void setRead_time(const QString &value);

    QString getRead_latitude(int i) const;
    void setRead_latitude(const QString &value);

    QString getRead_longitude(int i) const;
    void setRead_longitude(const QString &value);

    QString getRead_kmh(int i) const;
    void setRead_kmh(const QString &value);

    QString getRead_kts(int i) const;
    void setRead_kts(const QString &value);

    QString getRead_mph(int i) const;
    void setRead_mph(const QString &value);

    QString getRead_squwak(int i) const;
    void setRead_squwak(const QString &value);

    QString getRead_heading(int i) const;
    void setRead_heading(const QString &value);

    QString getRead_feet(int i) const;
    void setRead_feet(const QString &value);

    QString getRead_meters(int i) const;
    void setRead_meters(const QString &value);

    QString getRead_totalFeet(int i) const;
    void setRead_totalFeet(const QString &value);

    QString getRead_vvi(int i) const;
    void setRead_vvi(const QString &value);

    void clearAll();

    int size();

    void removeLast();

    void replaceRead_meters(int i, QString value);
private:
    QString read_flightId;
    QString read_code;
    QString read_model;
    QString read_tailNo;
    QString read_orgin;
    QString read_destination;
    QStringList read_date;
    QStringList read_time;
    QStringList read_latitude;
    QStringList read_longitude;
    QStringList read_kmh;
    QStringList read_kts;
    QStringList read_mph;
    QStringList read_squwak;
    QStringList read_heading;
    QStringList read_feet;
    QStringList read_meters;
    QStringList read_totalFeet;
    QStringList read_vvi;

    static flightData* instance;
    flightData();
};

#endif // FLIGHTDATA_H
