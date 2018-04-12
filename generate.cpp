#include "generate.h"

Generate::Generate()
{
//Auto generater few parametrs to run simulator properly

    flightData* fData = flightData::getInstance();
}

QString Generate::AutoGear(QString meterS){
    meters=meterS.toInt();
    if(meters>500){
        return "0,";
    }
    else{
        return "1,";
    }
}

QString Generate::AutoEngine(QString nowS,QString nowF,QString nowV){

    if(nowF.toDouble()==0)
        nowF="0.000001";

    power=(1+nowV.toDouble()/(nowF.toDouble()*2))*(50*nowS.toDouble()/aver);

    if(power>100)
        power=100;
    if(power<10)
        power=10;
    if(power!=power)
        power=10;

    return QString::number(power,'f',0)+","+QString::number(power,'f',0)+","+QString::number(power,'f',0)+","+QString::number(power,'f',0)+",";

}

void Generate::AutoMax(){
    aver=0;
    int zero=0;
    for(int i=0;i<fData->size();i++){
            if(fData->getRead_kts(i).toFloat()>100)
            aver+=1/((fData->getRead_kts(i).toFloat()));
            else
            zero++;
    }

    aver=(fData->size()-zero)/aver;
}

QString Generate::AutoHeading(QString latA, QString longA, QString latB, QString longB){
    double lat1=latA.toDouble();
    double lon1=longA.toDouble();
    double lat2=latB.toDouble();
    double lon2=longB.toDouble();
    lat1=qDegreesToRadians(lat1);
    lon1=qDegreesToRadians(lon1);
    lat2=qDegreesToRadians(lat2);
    lon2=qDegreesToRadians(lon2);
    double y = qSin(lon2-lon1)*qCos(lat2);
    double x = qCos(lat1)*qSin(lat2)-qSin(lat1)*qCos(lat2)*qCos(lon2-lon1);
    double head=0;

    head=qAtan2(y,x);
    head=qRadiansToDegrees(head);
    head=fmod((head+360.0),360.0);

    return QString::number(head,'f',1);

}

QString Generate::AutoRoll(QString head1, QString head2, QString meters){

    float he1=head1.toFloat();
    float he2=head2.toFloat();
    float met=meters.toFloat();
    float roll=he1-he2;
    if(met>10){

        if(roll>0 && roll<=180){
            roll*=(-1);
        }
        else if(roll<0 && roll>=(-180)){
            roll*=(-1);
        }
        else if(roll==0){
            roll=roll;
        }
        else if(roll>0 && roll>180){
            roll*=(-1);
            roll=fmodf(fmodf(roll,360)+360,360);
        }
        else if(roll<0 && roll<(-180)){
            roll=fmodf(fmodf(roll,360)+360,360);
            roll*=(-1);
        }
        else{
            roll=0;
        }

        if(roll>30){
            roll=30;
        }
        else if(roll<(-30)){
            roll=(-30);
        }


        return ","+QString::number(roll,'f',2)+",";
    }
    else{
        return ",0,";
    }
}

QString Generate::AutoKias(QString nowLatS,QString nowLongS,QString nowTimeS,QString nextLatS,QString nextLongS,QString nextTimeS){

    double Lat1=qDegreesToRadians(nowLatS.toDouble());
    double Lon1=qDegreesToRadians(nowLongS.toDouble());
    double Lat2=qDegreesToRadians(nextLatS.toDouble());
    double Lon2=qDegreesToRadians(nextLongS.toDouble());
    QDateTime Time2=QDateTime::fromString(nextTimeS, "yyyy-MM-ddTHH:mm:ss");
    QDateTime Time1=QDateTime::fromString(nowTimeS, "yyyy-MM-ddTHH:mm:ss");
    double Time2i= Time2.toTime_t();
    double Time1i=Time1.toTime_t();

    double x=qSin((Lat2-Lat1)/2)* qSin((Lat2-Lat1)/2)+qCos(Lat1) * qCos(Lat2) *qSin((Lon2-Lon1)/2) * qSin((Lon2-Lon1)/2);
    double y=2* atan2(sqrt(x), sqrt(1-x));
    double track=6371000*y;


    double diffTim=Time2i-Time1i;
    genKias=0.539956803*(3600*((track/1000)/diffTim));


    return QString::number(genKias,'f',4);

}

void Generate::AutoVvi(){

    double diffFeets;
    int diffTimes;
    double tempVvi;
    for(int i=0;i<fData->size();i++){
        if(i==fData->size()-1){
            fData->setRead_vvi("0");
        }
        else{
            diffFeets=fData->getRead_feet(i+1).toDouble()-fData->getRead_feet(i).toDouble();
            diffTimes=QDateTime::fromString(QString(fData->getRead_date(i+1))+"T"+QString(fData->getRead_time(i+1)), "yyyy/MM/ddTHH:mm:ss").toTime_t()-QDateTime::fromString(QString(fData->getRead_date(i))+"T"+QString(fData->getRead_time(i)), "yyyy/MM/ddTHH:mm:ss").toTime_t();
            tempVvi=60*(diffFeets/diffTimes);
            fData->setRead_vvi(QString::number(tempVvi,'f',2));
        }

    }
}


QString Generate::AutoVviKml(QString Feet1, QString Feet2, QString Time1, QString Time2){
    double diffFeets;
    int diffTimes;
    double tempVvi;

    diffFeets=Feet2.toDouble()-Feet1.toDouble();
    diffTimes=QDateTime::fromString(QString(Time2), "yyyy-MM-ddTHH:mm:ss").toTime_t()-QDateTime::fromString(QString(Time1), "yyyy-MM-ddTHH:mm:ss").toTime_t();
    tempVvi=60*(diffFeets/diffTimes);


    return QString::number(tempVvi,'f',2);

}
