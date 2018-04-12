#ifndef GENERATE_H
#define GENERATE_H
#include <QString>
#include <QStringList>
#include <QDebug>
#include <QtMath>
#include <QDateTime>
#include <flightdata.h>



class Generate
{
public:
    Generate();


    QString AutoGear(QString);
    QString AutoEngine(QString,QString,QString);
    void AutoMax();
    QString AutoHeading(QString,QString,QString,QString);
    QString AutoRoll(QString,QString,QString);
    QString AutoKias(QString,QString,QString,QString,QString,QString);
    void AutoVvi();
    QString AutoVviKml(QString,QString,QString,QString);

private:
    int meters,power;
    float aver;
    double genKias;
    flightData* fData;

};

#endif // GENERATE_H
