#ifndef PITCHCALC_H
#define PITCHCALC_H
#include <QString>
#include <QtMath>
#include <QDebug>


class PitchCalc
{
public:
    PitchCalc();
    QString getPitch(QString,QString,QString,QString,QString,QString);
private:
    double Lat1;
    double Lon1;
    double Meters1;
    double Lat2;
    double Lon2;
    double Meters2;
    double track;
    double diffMet;
    double pitchCalc;
    double x;
    double y;
    const double earthR = 6371000;
};

#endif // PITCHCALC_H
