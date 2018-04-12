#include "pitchcalc.h"

PitchCalc::PitchCalc()
{

}

QString PitchCalc::getPitch(QString nowLatS,QString nowLongS,QString nowMetersS,QString nextLatS,QString nextLongS,QString nextMetersS){

    Lat1=qDegreesToRadians(nowLatS.toDouble());
    Lon1=qDegreesToRadians(nowLongS.toDouble());
    Meters1=nowMetersS.toDouble();

    Lat2=qDegreesToRadians(nextLatS.toDouble());
    Lon2=qDegreesToRadians(nextLongS.toDouble());
    Meters2=nextMetersS.toDouble();

    //distance beetwen two points

    x=qSin((Lat2-Lat1)/2)* qSin((Lat2-Lat1)/2)+qCos(Lat1) * qCos(Lat2) *qSin((Lon2-Lon1)/2) * qSin((Lon2-Lon1)/2);
    y=2* atan2(sqrt(x), sqrt(1-x));
    track=earthR*y;



    diffMet=Meters2-Meters1;
    pitchCalc=diffMet/track;

    pitchCalc=qRadiansToDegrees(pitchCalc);
    if(pitchCalc>50||pitchCalc<-50){
        pitchCalc=0;
    }
    if(pitchCalc!=pitchCalc){
        pitchCalc=0;
    }
    if(Meters2-Meters1<0 && Meters1<300){
        pitchCalc*=(-1);
    }
    if(Meters1<10 && pitchCalc>10){
        pitchCalc=10;
    }



return QString::number(pitchCalc,'f',2);
}
