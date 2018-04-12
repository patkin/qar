#include "core.h"
#include "ui_mainwindow.h"

Core::Core(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    conf= new Options; //Options class
    conf->ReadOptions(); //Load options
    planePitch=new PitchCalc; // calculate pitch parametr
    gen= new Generate;  //quick generate basic plane parametr
    api= new GoogleApi; //api - google information about elevation
    //Views:
    loading = new Wait; //loading screen
    about= new Info;    // information screen
    plot= new FlightPlot; // flight data plots
    stream.setCodec("IBM 850");


    flightData* fData = flightData::getInstance(); //flight data contener

    //Signal- slots connections
    connect(ui->pushButton,SIGNAL(clicked(bool)),this,SLOT(Simulate()));
    connect(ui->pushButton,SIGNAL(clicked(bool)),this,SLOT(SimulatedMsgOpt()));
    connect(ui->pushButton_2,SIGNAL(clicked(bool)),conf,SLOT(show()));
    connect(ui->pushButton_2,SIGNAL(clicked(bool)),conf,SLOT(ReadOptions()));
    connect(ui->pushButton_3,SIGNAL(clicked(bool)),this,SLOT(GenerateFDR()));
    connect(ui->pushButton_4,SIGNAL(clicked(bool)),this,SLOT(close()));
    connect(this,SIGNAL(errorS()),this,SLOT(error()));



}

Core::~Core()
{
    delete ui;
}

//Controls
void Core::SimulatedMsgOpt(){
    if(conf->badOptions){
        QMessageBox msgBox;
        msgBox.setText("Before you first time use a simulated mode configure program.\n If you click ok button you go to options menu.");
        msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Ok);
        int ret=msgBox.exec();
        switch (ret) {
        case QMessageBox::Ok:
            conf->show();
            break;
        default:
            break;
        }
    }
}

void Core::startSimulator(){
     QString start="start ";
     QString args=" --fps_test=1 --load_smo=Output/QARPlane.fdr";
     QString str1 ='"'+conf->source+'"'+args;

     QByteArray ba = str1.toLatin1();
     const char *c_str2 = ba.data();

     system (c_str2);
}

void Core::Simulate(){

    ClearMemoryRead();

    if(!(conf->badOptions)){
        if(ReadFile()){
            if(fileReadGood){
                loop.connect(plot, SIGNAL(Done()), SLOT(quit()));
                loop.connect(plot,SIGNAL(rejected()),SLOT(quit()));
                loop.exec();

                if(plot->checkStatus()){

                    if(txtKml==1){
                        loading->show();
                        if(TxtFileToFdr(true)){
                            loading->close();
                            startSimulator();
                        }
                    }
                    else if(txtKml==2){
                        loading->show();
                        if(KmlFileToFdr(true)){
                            loading->close();
                            startSimulator();
                        }
                    }

                }

            }
        }
    }

}

void Core::GenerateFDR(){
    ClearMemoryRead();
    if(ReadFile()){

        loop.connect(plot, SIGNAL(Done()), SLOT(quit()));
        loop.connect(plot,SIGNAL(rejected()),SLOT(quit()));
        loop.exec();

        if(plot->checkStatus()){
            if(txtKml==1){
                TxtFileToFdr(false);
            }
            else if (txtKml==2){
                KmlFileToFdr(false);
            }
        }
        else{
            ClearMemoryRead();
        }
    }

}

void   Core::closeEvent(QCloseEvent*)
{
    qApp->quit();
}

void Core::on_actionExit_triggered()
{
    this->close();
}

void Core::on_actionOptions_triggered()
{
    conf->show();
}

void Core::on_actionInfo_triggered()
{
    about->show();
}

/*ReadFile section
 * read txt file from flightradar and secound method - kml file from fligtaware*/


bool Core::ReadFile(){
        ClearMemoryRead();
        fileReadGood=false;
        fileName=QFileDialog::getOpenFileName(this,tr("Open..."), "/home/", tr("Flight Data Text File (*.txt);;Flight Data KML File (*.kml)"));
        QFileInfo fi(fileName);
        loading->show();
        if(fi.suffix()=="kml"){
            txtKml=2;
            return ReadFileKml(fileName);
        }
        else if(fi.suffix()=="txt"){
         //TXT READ FILE SECTION---------------------------------------------------------------
                txtKml=1;
                QFile file(fileName);
                   if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
                       return false;

                   while (!file.atEnd()) {
                       line = file.readLine();
                       //check file
                       if((line.contains("# flightId:"))|| (fileReadGood)){
                        fileReadGood=true;
                       }
                       else{
                           QMessageBox msgBox;
                           msgBox.setText("The document is bad");
                           msgBox.exec();
                           return false;
                       }
                       // reading information from file
                       QRegExp sep("\\s+");
                       QRegExp digitDate("(\\d{4})/(\\d{2})/(\\d{2}) (\\d{2}):(\\d{2}):(\\d{2})");
                       if(line.contains("# flightId:")){
                           fData->setRead_flightId(line.section(sep,2,2));

                       }
                       if(line.contains("# code:")){
                           fData->setRead_code(line.section(sep,2,3));

                       }
                       if(line.contains("# model:")){
                           fData->setRead_model(line.section(sep,2,2));

                       }
                       if(line.contains("# tailno:")){
                           fData->setRead_tailNo(line.section(sep,2,2));

                       }
                       if(line.contains("# origin:")){
                           fData->setRead_orgin(line.section(sep,-2,-2));

                       }
                       if(line.contains("# destination:")){
                           fData->setRead_destination(line.section(sep,-2,-2));

                       }
                       if(digitDate.indexIn(line)==0){
                           fData->setRead_date(line.section(sep,0,0));
                           fData->setRead_time(line.section(sep,1,1));
                           fData->setRead_latitude(line.section(sep,2,2));
                           fData->setRead_longitude(line.section(sep,3,3));
                           fData->setRead_kmh(line.section(sep,4,4));
                           fData->setRead_kts(line.section(sep,5,5));
                           fData->setRead_mph(line.section(sep,6,6));
                           fData->setRead_squwak(line.section(sep,7,7));
                           fData->setRead_heading(line.section(sep,8,8));
                           fData->setRead_feet(line.section(sep,9,9));
                           fData->setRead_meters(line.section(sep,10,10));
                       }


                   }

                   req="";
                   int j=0;
                   int l=0;

                   for(int i=0;i<fData->size();i++){


                       if(j<=100){

                           if(i<fData->size()-1 && j!=100)
                               req+=fData->getRead_latitude(i)+","+fData->getRead_longitude(i)+"|";
                           else if(j==100)
                               req+=fData->getRead_latitude(i)+","+fData->getRead_longitude(i);
                           else{
                               req+=fData->getRead_latitude(i)+","+fData->getRead_longitude(i);
                               j=100;
                           }

                           j++;
                       }
                       if(j>100){

                           read_api=api->httpReq(req);
                           if(api->getConnectionStatus()){
                               for(int k=0;k<read_api.size();k++){
                                   totalFeetF=(QString(read_api.at(k)).toFloat())+(QString(fData->getRead_feet(l)).toFloat());
                                   totalFeet=QString::number(totalFeetF,'f',4);

                                   fData->setRead_totalFeet(totalFeet);
                                   l++;
                               }
                               j=0;
                               req="";
                           }
                           else{
                               loading->hide();
                               QMessageBox msgBox;
                               msgBox.setText("Somthing goes wrong with google maps connection.");
                               msgBox.exec();
                               return false;
                           }

                           read_api.clear();

                       }

                   }

                   gen->AutoVvi();


                   file.close();
                   plot->setPlots(1);
                   loading->hide();
                   return true;

        }  //END TXT READ SECTION-------------------------------------------------------------------------------------------
        else{
            loading->hide();
            QMessageBox msgBox;
            msgBox.setText("Somthing goes wrong, try once again or use different file.");
            msgBox.exec();
            return false;
        }
}
bool Core::ReadFileKml(QString fileNameK){
    QFile file(fileNameK);
       if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
           loading->hide();
           QMessageBox msgBox;
           msgBox.setText("Somthing goes wrong, try once again or use different file.");
           msgBox.exec();
           return false;
       }
       else{
            QXmlStreamReader xml(&file);
            QString temp;
            QRegExp sep("\\s+");
            kmlType=0;


            if(!xml.atEnd() && !xml.hasError()){

                for(int i=0;i<6;i++){
                    xml.readNext();
                }
                if(xml.name().toString()=="name"){

                    QString type=xml.readElementText();

                    if(type.contains("FlightAware")){
                        while (!xml.atEnd() && !xml.hasError())
                        {
                            xml.readNext();


                            if (xml.isStartElement())
                            {
                                QString name = xml.name().toString();

                                if(name == "coord"){
                                    temp=xml.readElementText();
                                    fData->setRead_longitude(temp.section(sep,0,0));
                                    //qDebug()<<"read: "<<temp.section(sep,1,1);
                                    fData->setRead_latitude(temp.section(sep,1,1));
                                    fData->setRead_meters(temp.section(sep,2,2));

                                }
                                if(name == "when"){
                                    fData->setRead_date(xml.readElementText());

                                }
                                if(name == "name"){
                                    fData->setRead_squwak(xml.readElementText());

                                }
                            }
                        }
                        if (xml.hasError())
                        {
                            QMessageBox msgBox5;
                            loading->hide();
                            msgBox5.setText("Somthing goes wrong, try once again or use different file.");
                            msgBox5.exec();
                            return false;
                        }
                        kmlType=1;

                    }
                    else if(type.contains("test")){

                        int line=5;
                        while (!xml.atEnd() && !xml.hasError())
                        {
                            xml.readNext();


                            if (xml.isStartElement())
                            {
                                QString name = xml.name().toString();

                                if(name=="Placemark"){
                                    while (!xml.atEnd() && !xml.hasError()) {
                                          xml.readNext();
                                          QString name = xml.name().toString();
                                          if(name=="name"){
                                              if(line==5)
                                              fData->setRead_time(xml.readElementText());
                                          }
                                          if(name == "coordinates"){
                                              temp=xml.readElementText();

                                              if(line==5){
                                                  double tempLong=QString(temp.section(",",0,0)).toDouble();
                                                  fData->setRead_longitude(QString::number(tempLong,'f',6));
                                                  double tempLati=QString(temp.section(",",1,1)).toDouble();
                                                  fData->setRead_latitude(QString::number(tempLati,'f',6));
                                                  double tempMeters=QString(temp.section(",",2,2)).toDouble();
                                                  fData->setRead_meters(QString::number(tempMeters,'f',0));
                                                  line=1;
                                              }
                                              else{
                                                  line++;
                                              }


                                              break;
                                          }

                                    }
                                }


                            }
                        }
                        if (xml.hasError())
                        {
                            QMessageBox msgBox;
                            loading->hide();
                            msgBox.setText("Somthing goes wrong, file structure error.");
                            msgBox.exec();
                            return false;
                        }
                        fData->removeLast();


                        kmlType=2;

                    }
                    else{
                        QMessageBox msgBox;
                        loading->hide();
                        msgBox.setText("Somthing goes wrong, try once again or use different file.");
                        msgBox.exec();
                        return false;
                    }
                }

            }


            if(fData->size()<1){
                QMessageBox msgBox;
                loading->hide();
                msgBox.setText("Wrong file, try use different.");
                msgBox.exec();
                return false;
            }

            if(kmlType==2){
                int tempMeters=QString(fData->getRead_meters(0)).toInt();
                int tempMeters2=0;
                for(int i=0;i<fData->size();i++){
                    tempMeters2=QString(fData->getRead_meters(i)).toInt()-tempMeters;
                    fData->replaceRead_meters(i,QString::number(tempMeters2));
                }
            }

            for(int x=0;x<fData->size();x++){
                fData->setRead_feet(QString::number((QString(fData->getRead_meters(x)).toFloat()*3.280839895),'f',4));
            }


            if(kmlType==1 || kmlType==2){
                for(int k=0;k<fData->size();k++){

                    if(k==fData->size()-1)
                        fData->setRead_heading(gen->AutoHeading(fData->getRead_latitude(k-1),fData->getRead_longitude(k-1),fData->getRead_latitude(k),fData->getRead_longitude(k)));
                    else
                        fData->setRead_heading(gen->AutoHeading(fData->getRead_latitude(k),fData->getRead_longitude(k),fData->getRead_latitude(k+1),fData->getRead_longitude(k+1)));
                }
            }
            else if(kmlType==3){
                for(int k=0;k<fData->size();k++){

                    if(k>=fData->size()-10){
                        double tempLat=0;
                        double tempLong=0;
                        for(int z=11;z>0;z--){
                            tempLat+=QString(fData->getRead_latitude(k-z)).toDouble();
                            tempLong+=QString(fData->getRead_longitude(k-z)).toDouble();
                        }
                        tempLat/=10;
                        tempLong/=10;
                        QString tempHead=gen->AutoHeading(QString::number(tempLat,'f',8),QString::number(tempLong,'f',8),fData->getRead_latitude(k),fData->getRead_longitude(k));
                        double tempHeadD=tempHead.toDouble();
                        tempHeadD=fmod((tempHeadD+180),360);
                        fData->setRead_heading(QString::number(tempHeadD,'f',8));
                    }
                    else{
                        double tempLat=0;
                        double tempLong=0;
                        for(int z=1;z<11;z++){
                            tempLat+=QString(fData->getRead_latitude(k+z)).toDouble();
                            tempLong+=QString(fData->getRead_longitude(k+z)).toDouble();
                        }
                        tempLat/=10;
                        tempLong/=10;

                        fData->setRead_heading(gen->AutoHeading(fData->getRead_latitude(k),fData->getRead_longitude(k),QString::number(tempLat,'f',8),QString::number(tempLong,'f',8)));
                    }

                }
            }

            if(kmlType==1){

                req="";
                int j=0;
                int l=0;

                for(int i=0;i<fData->size();i++){

                    if(j<=50){

                        if(i<fData->size()-1 && j!=50)
                            req+=fData->getRead_latitude(i)+","+fData->getRead_longitude(i)+"|";
                        else if(j==50)
                            req+=fData->getRead_latitude(i)+","+fData->getRead_longitude(i);
                        else{
                            req+=fData->getRead_latitude(i)+","+fData->getRead_longitude(i);
                            j=50;
                        }

                        j++;
                    }
                    if(j>50){

                        read_api=api->httpReq(req);
                        if(api->getConnectionStatus()){
                            for(int k=0;k<read_api.size();k++){
                                totalFeetF=(QString(read_api.at(k)).toFloat())+((QString(fData->getRead_feet(l)).toFloat()));
                                totalFeet=QString::number(totalFeetF,'f',4);

                                fData->setRead_totalFeet(totalFeet);
                                l++;
                            }
                            j=0;
                            req="";
                        }
                        else{
                            QMessageBox msgBox;
                            loading->hide();
                            msgBox.setText("Somthing goes wrong with google maps connection.");
                            msgBox.exec();
                            return false;
                        }


                        read_api.clear();

                    }
                }

            }
            else if(kmlType==2){
                for(int i=0;i<fData->size();i++)
                    fData->setRead_totalFeet(fData->getRead_feet(i));
            }



            if(kmlType==1){
                for(int i=0;i<fData->size();i++){
                    if(i==fData->size()-1){
                        fData->setRead_kts(fData->getRead_kts(i-1));
                        fData->setRead_vvi("0");
                    }
                    else{
                        fData->setRead_kts(gen->AutoKias(fData->getRead_latitude(i),fData->getRead_longitude(i),QString(fData->getRead_date(i)),fData->getRead_latitude(i+1),fData->getRead_longitude(i+1),QString(fData->getRead_date(i+1))));
                        fData->setRead_vvi(gen->AutoVviKml(fData->getRead_totalFeet(i),fData->getRead_totalFeet(i+1),fData->getRead_date(i),fData->getRead_date(i+1)));
                    }
                }
            }
            else if(kmlType==2){
                for(int i=0;i<fData->size();i++){
                    QDateTime::fromString("2000-01-01T"+QString(fData->getRead_time(0)), "yyyy-MM-ddTHH:mm:ss").toTime_t();


                    if(i==fData->size()-1){
                        fData->setRead_kts(fData->getRead_kts(i-1));
                        fData->setRead_vvi("0");
                    }
                    else{
                        if(QDateTime::fromString("2000-01-01T"+QString(fData->getRead_time(i)), "yyyy-MM-ddTHH:mm:ss").toTime_t()<QDateTime::fromString("2000-01-01T"+QString(fData->getRead_time(i+1)), "yyyy-MM-ddTHH:mm:ss").toTime_t()){
                            fData->setRead_kts(gen->AutoKias(fData->getRead_latitude(i),fData->getRead_longitude(i),"2000-01-01T"+QString(fData->getRead_time(i)),fData->getRead_latitude(i+1),fData->getRead_longitude(i+1),"2000-01-01T"+QString(fData->getRead_time(i+1))));
                            fData->setRead_vvi(gen->AutoVviKml(fData->getRead_totalFeet(i),fData->getRead_totalFeet(i+1),"2000-01-01T"+QString(fData->getRead_time(i)),"2000-01-01T"+QString(fData->getRead_time(i+1))));
                        }
                        else{
                            fData->setRead_kts(gen->AutoKias(fData->getRead_latitude(i),fData->getRead_longitude(i),"2000-01-01T"+QString(fData->getRead_time(i)),fData->getRead_latitude(i+1),fData->getRead_longitude(i+1),"2000-01-02T"+QString(fData->getRead_time(i+1))));
                            fData->setRead_vvi(gen->AutoVviKml(fData->getRead_totalFeet(i),fData->getRead_totalFeet(i+1),"2000-01-01T"+QString(fData->getRead_time(i)),"2000-01-02T"+QString(fData->getRead_time(i+1))));

                        }
                    }
                }
            }



       }

       fileReadGood=true;
       file.close();
       if(kmlType==1)
           plot->setPlots(2);
       else if(kmlType==2)
           plot->setPlots(3);
       loading->hide();
       return true;

}

//Create fdr file section, read documentation to check fdr specyfication and parametrs order
bool Core::TxtFileToFdr(bool sim){

    loading->show();
    if(!sim){
        fileNameS = QFileDialog::getSaveFileName(this,tr("Save as..."),tr("/home/"),tr("Flight Data Recorder File(*.fdr)"));
    }
    else{
        fileNameS = conf->sourceFolder+"/Output/QARPlane.fdr";
    }


        if(fileNameS.isEmpty())
            return false;

        fileSave.setFileName(fileNameS);
        fileSave.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text);

        stream.setDevice(&fileSave);
        stream<< "I\n1\nCOMM, File generated by QAR-Plane created by Patrick Kindla\n\nCOMM, Flight id: "+fData->getRead_flightId()+"\nCOMM, Airline and Line: "+fData->getRead_code()+"\nCOMM, Airplane Model: "+fData->getRead_model()+"\nCOMM, Origin "+fData->getRead_orgin()+"\nCOMM, Destination "+fData->getRead_destination()+"\n\n";
        stream<< "\nACFT,"+conf->model;
        stream<< "\nTAIL,"+fData->getRead_tailNo()+",";
        stream<< "\nDATE,"+QString (fData->getRead_date(0)).mid(8,2)+"/"+QString (fData->getRead_date(0)).mid(5,2)+"/"+QString (fData->getRead_date(0)).mid(0,4)+",";
        stream<< "\nPRES,29.83,\nTEMP,65,\nWIND,230,16,";
        stream<< "\nTIME,"+QString(fData->getRead_time(0))+",";
        if(fData->getRead_meters(0)=="0"){
            stream<<"\nCALI,"+fData->getRead_longitude(0)+","+fData->getRead_latitude(0)+","+fData->getRead_totalFeet(0)+",";
        }

        QString marks=plot->getMarks();
        if(!marks.isEmpty()){
            stream<<marks+" \n\n";
        }
        int start=plot->getStart();
        int stop=plot->getStop();

        date1=QDateTime::fromString(QString(fData->getRead_date(0))+"T"+QString(fData->getRead_time(0)), "yyyy/MM/ddTHH:mm:ss");
        timeBegin=date1.toTime_t();

        if(conf->genPower.contains("true"))
            gen->AutoMax();

        for(int i=0;i<fData->size();i++){
            date2=QDateTime::fromString(QString(fData->getRead_date(i))+"T"+QString(fData->getRead_time(i)), "yyyy/MM/ddTHH:mm:ss");
            timeNow=date2.toTime_t();

            timeDiff=timeNow-timeBegin;

            if((timeDiff>=start) && (timeDiff<=stop)){

                if((fData->size()-2>i)&& fData->getRead_meters(i)!="0"){

                    GenerateToFixBug(i);
                }
                else{
                    GenerateIf(i);
                    for(int k=0;k<2;k++){
                        stream<<"\nDATA, "+QString::number(timeDiff)+",65.0,"+lon+","+lat+","+fData->getRead_totalFeet(i)+", "+fData->getRead_feet(i)+",0.0,0.0,0.0,"+gen_pitch+gen_roll+fData->getRead_heading(i)+", "+fData->getRead_kts(i)+","+fData->getRead_vvi(i)+",0.0,0.0,0.0,0.0,0.0, "+"0.0,0.0,0.0,0.0,"+gen_gear+gen_gear+gen_gear+gen_gear+"0.1, 11010,10930,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,1.0,1.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,8.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0, 29.92,0.0,0.0,0.0,0.0,2.0,2.0,1.0,1.0,1.0,1.0,"+gen_power+gen_power+"1.2,1.2,1.2,1.2, 600.0,600.0,600.0,600.0, 5000.0,5000.0,5000.0,5000.0, 0.0,0.0,0.0,0.0, 0.0,0.0,0.0,0.0, 0.0,0.0,0.0,0.0";

                    }
                }
            }

            stream.flush();
        }

        fileSave.close();
        ClearMemoryRead();

        if(!sim && !(stream.status() == QTextStream::WriteFailed)){
            QMessageBox msgBox8;
            loading->hide();
            msgBox8.setText("Generated complete!");
            msgBox8.exec();
        }
        else if(stream.status() == QTextStream::WriteFailed){
            emit errorS();
            loading->hide();
            return false;
        }
        loading->hide();
        return true;
}

bool Core::KmlFileToFdr(bool sim){

    loading->show();
    if(!sim){
        fileNameS = QFileDialog::getSaveFileName(this,tr("Save as..."),tr("/home/"),tr("Flight Data Recorder File(*.fdr)"));
    }
    else{
        fileNameS = conf->sourceFolder+"/Output/QARPlane.fdr";
    }


        if(fileNameS.isEmpty())
            return false;

        fileSave.setFileName(fileNameS);
        fileSave.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text);

        stream.setDevice(&fileSave);

        if(conf->genPower.contains("true"))
            gen->AutoMax();

        if(kmlType==1)
            stream<< "A\n1\nCOMM, File generated by QAR-Plane\n\nCOMM, Flight id: "+fData->getRead_squwak(3)+"\nCOMM, Origin: "+fData->getRead_squwak(1)+"\nCOMM, Destination: "+fData->getRead_squwak(2)+"\n\n";
        else if(kmlType==2)
            stream<< "A\n1\nCOMM, File generated by QAR-Plane\n\n";
        stream<< "\nACFT,"+conf->model;
        stream<< "\nTAIL,0000,";
        if(kmlType==1)
            stream<< "\nDATE,"+QString (fData->getRead_date(0)).mid(8,2)+"/"+QString (fData->getRead_date(0)).mid(5,2)+"/"+QString (fData->getRead_date(0)).mid(0,4)+",";
        stream<< "\nPRES,29.83,\nTEMP,65,\nWIND,230,16,";
        if(kmlType==1)
            stream<< "\nTIME,"+QString(fData->getRead_date(0)).mid(11,8)+",";
        else if(kmlType==2)
            stream<< "\nTIME,"+QString(fData->getRead_time(0))+",";
        if(fData->getRead_meters(0)=="0"){
            stream<<"\nCALI,"+fData->getRead_longitude(0)+","+fData->getRead_latitude(0)+","+fData->getRead_totalFeet(0)+",";
        }

        QString marks=plot->getMarks();
        if(!marks.isEmpty()){
            stream<<marks+" \n\n";
        }

        int start=plot->getStart();
        int stop=plot->getStop();

        if(kmlType==1){
            date1=QDateTime::fromString(QString(fData->getRead_date(0)), "yyyy-MM-ddTHH:mm:ss");
            timeBegin=date1.toTime_t();
        }
        else if(kmlType==2){
            date1=QDateTime::fromString("2000-01-01T"+QString(fData->getRead_time(0)), "yyyy-MM-ddTHH:mm:ss");
            timeBegin=date1.toTime_t();
        }





        for(int i=0;i<fData->size();i++){


            if(kmlType==1){
                date2=QDateTime::fromString(QString(fData->getRead_date(i)), "yyyy-MM-ddTHH:mm:ss");
                timeNow=date2.toTime_t();
            }
            else if(kmlType==2){
                date2=QDateTime::fromString("2000-01-01T"+QString(fData->getRead_time(i)), "yyyy-MM-ddTHH:mm:ss");
                timeNow=date2.toTime_t();

                if(timeNow<timeBegin){
                    date2=QDateTime::fromString("2000-01-02T"+QString(fData->getRead_time(i)), "yyyy-MM-ddTHH:mm:ss");
                    timeNow=date2.toTime_t();
                }
            }




            timeDiff=timeNow-timeBegin;
            if((timeDiff>=start) && (timeDiff<=stop)){

                if(kmlType==1){
                    if((fData->size()-2>i)&& fData->getRead_feet(i)!="0"){
                        GenerateToFixBugKml(i);
                    }
                    else{
                        GenerateIf(i);
                        for(int x=0;x<2;x++){
                            stream<<"\nDATA, "+QString::number(timeDiff)+",65.0,"+lon+","+lat+","+fData->getRead_totalFeet(i)+", "+fData->getRead_feet(i)+",0.0,0.0,0.0,"+gen_pitch+gen_roll+fData->getRead_heading(i)+", "+fData->getRead_kts(i)+","+fData->getRead_vvi(i)+",0.0,0.0,0.0,0.0,0.0, "+"0.0,0.0,0.0,0.0,"+gen_gear+gen_gear+gen_gear+gen_gear+"0.1, 11010,10930,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,1.0,1.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,8.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0, 29.92,0.0,0.0,0.0,0.0,2.0,2.0,1.0,1.0,1.0,1.0,"+gen_power+gen_power+"1.2,1.2,1.2,1.2, 600.0,600.0,600.0,600.0, 5000.0,5000.0,5000.0,5000.0, 0.0,0.0,0.0,0.0, 0.0,0.0,0.0,0.0, 0.0,0.0,0.0,0.0";
                        }
                    }
                }
                else if(kmlType==2){
                    GenerateIf(i);
                    for(int x=0;x<2;x++){
                        stream<<"\nDATA, "+QString::number(timeDiff)+",65.0,"+lon+","+lat+","+fData->getRead_totalFeet(i)+", "+fData->getRead_feet(i)+",0.0,0.0,0.0,"+gen_pitch+gen_roll+fData->getRead_heading(i)+", "+fData->getRead_kts(i)+","+fData->getRead_vvi(i)+",0.0,0.0,0.0,0.0,0.0, "+"0.0,0.0,0.0,0.0,"+gen_gear+gen_gear+gen_gear+gen_gear+"0.1, 11010,10930,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,1.0,1.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,8.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0, 29.92,0.0,0.0,0.0,0.0,2.0,2.0,1.0,1.0,1.0,1.0,"+gen_power+gen_power+"1.2,1.2,1.2,1.2, 600.0,600.0,600.0,600.0, 5000.0,5000.0,5000.0,5000.0, 0.0,0.0,0.0,0.0, 0.0,0.0,0.0,0.0, 0.0,0.0,0.0,0.0";
                    }
                }
            }
            stream.flush();
        }


        fileSave.close();
        ClearMemoryRead();
        if(!sim && !(stream.status() == QTextStream::WriteFailed)){
            QMessageBox msgBox8;
            loading->hide();
            msgBox8.setText("Generated complete!");
            msgBox8.exec();
        }
        else if(stream.status() == QTextStream::WriteFailed){
            emit errorS();
            loading->hide();
            return false;
        }

        return true;

}

//Bug fix section

void Core::GenerateToFixBug(int i){
    lonf=QString(fData->getRead_longitude(i)).toFloat();
    lonf2=QString(fData->getRead_longitude(i+1)).toFloat();
    latf=QString(fData->getRead_latitude(i)).toFloat();
    latf2=QString(fData->getRead_latitude(i+1)).toFloat();


    date2=QDateTime::fromString(QString(fData->getRead_date(i+1))+"T"+QString(fData->getRead_time(i+1)), "yyyy/MM/ddTHH:mm:ss");
    timeNow=date2.toTime_t();
    float timeDiff2=timeNow-timeBegin;


    GenerateToFixBug2(latf,lonf,latf2,lonf2,timeDiff,timeDiff2,i);

}

void Core::GenerateToFixBug2(float lat1,float long1,float lat2, float long2,float time1, float time2, int i){

    GenerateIf(i);



    stream<<"\nDATA, "+QString::number(timeDiff)+",65.0,"+lon+","+lat+","+fData->getRead_totalFeet(i)+", "+fData->getRead_feet(i)+",0.0,0.0,0.0,"+gen_pitch+gen_roll+fData->getRead_heading(i)+", "+fData->getRead_kts(i)+","+fData->getRead_vvi(i)+",0.0,0.0,0.0,0.0,0.0, "+"0.0,0.0,0.0,0.0,"+gen_gear+gen_gear+gen_gear+gen_gear+"0.1, 11010,10930,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,1.0,1.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,8.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0, 29.92,0.0,0.0,0.0,0.0,2.0,2.0,1.0,1.0,1.0,1.0,"+gen_power+gen_power+"1.2,1.2,1.2,1.2, 600.0,600.0,600.0,600.0, 5000.0,5000.0,5000.0,5000.0, 0.0,0.0,0.0,0.0, 0.0,0.0,0.0,0.0, 0.0,0.0,0.0,0.0";
    stream<<"\nDATA, "+QString::number(timeDiff)+",65.0,"+lon+","+lat+","+fData->getRead_totalFeet(i)+", "+fData->getRead_feet(i)+",0.0,0.0,0.0,"+gen_pitch+gen_roll+fData->getRead_heading(i)+", "+fData->getRead_kts(i)+","+fData->getRead_vvi(i)+",0.0,0.0,0.0,0.0,0.0, "+"0.0,0.0,0.0,0.0,"+gen_gear+gen_gear+gen_gear+gen_gear+"0.1, 11010,10930,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,1.0,1.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,8.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0, 29.92,0.0,0.0,0.0,0.0,2.0,2.0,1.0,1.0,1.0,1.0,"+gen_power+gen_power+"1.2,1.2,1.2,1.2, 600.0,600.0,600.0,600.0, 5000.0,5000.0,5000.0,5000.0, 0.0,0.0,0.0,0.0, 0.0,0.0,0.0,0.0, 0.0,0.0,0.0,0.0";


    QList<float> latList;
    QList<float> longList;
    QList<float> timeList;
    QStringList headList;
    QStringList pitchList;
    QStringList rollList;
    QList<float> metersList;

    float metersDiff=QString(fData->getRead_meters(i+1)).toFloat()-QString(fData->getRead_meters(i)).toFloat();



         lat1=qDegreesToRadians(lat1);
         long1=qDegreesToRadians(long1);
         lat2=qDegreesToRadians(lat2);
         long2=qDegreesToRadians(long2);

         float time3Diff=time2-time1;
         float timeIn;
         float scale=1;
         bool interpolate=true;

         if(time3Diff<5)
            interpolate=false;
         else if(time3Diff>=5 && time3Diff<10)
            scale=0.5;
         else
            scale=(1/time3Diff)*2;
         //*5
         //qDebug()<<scale;


         float d=0.1;

         if(interpolate){

             for(float f=0;f<1;f+=scale){
                 float A=qSin((1-f)*d)/qSin(d);
                 float B=qSin(f*d)/qSin(d);
                 float x = A*qCos(lat1)*cos(long1) +  B*qCos(lat2)*cos(long2);
                 float y = A*qCos(lat1)*sin(long1) +  B*qCos(lat2)*sin(long2);
                 float z = A*qSin(lat1)           +  B*qSin(lat2);
                 float lat3=qAtan2(z,sqrt((powf(x,2))+(powf(y,2))));
                 float long3=qAtan2(y,x);

                 lat3=qRadiansToDegrees(lat3);
                 long3=qRadiansToDegrees(long3);

                 latList.append(lat3);
                 longList.append(long3);
             }


             for(int k=1;k<latList.size()+1;k++){
                 float metersIn=(k*scale)*metersDiff;
                 metersList.append(QString(fData->getRead_meters(i)).toFloat()+metersIn);
             }


             for(int k=0;k<latList.size();k++){
                 if(k<latList.size()-1)
                     headList.append(gen->AutoHeading(QString::number(latList.at(k),'f',4),QString::number(longList.at(k),'f',4),QString::number(latList.at(k+1),'f',4),QString::number(longList.at(k+1),'f',4)));
                 else
                     headList.append(gen->AutoHeading(QString::number(latList.at(k),'f',4),QString::number(longList.at(k),'f',4),QString::number(latList.at(k),'f',4),QString::number(longList.at(k),'f',4)));
             }

             if(conf->genPitch.contains("true")){
                 for(int k=0;k<latList.size();k++){
                     if(k<latList.size()-1)
                         pitchList.append(planePitch->getPitch(QString::number(latList.at(k),'f',4),QString::number(longList.at(k),'f',4),QString::number(metersList.at(k),'f',4),QString::number(latList.at(k+1),'f',4),QString::number(longList.at(k+1),'f',4),QString::number(metersList.at(k+1),'f',4)));

                     else
                         pitchList.append(planePitch->getPitch(QString::number(latList.at(k),'f',4),QString::number(longList.at(k),'f',4),QString::number(metersList.at(k),'f',4),QString::number(latf2,'f',4),QString::number(lonf2,'f',4),fData->getRead_meters(i+1)));
                 }
             }
             else{
                 for(int k=0;k<latList.size();k++){
                     pitchList.append("0");
                 }
             }

             if(conf->genRoll.contains("true")){
                 for(int k=0;k<latList.size();k++){
                     if(k<latList.size()-1)
                         rollList.append(gen->AutoRoll(headList.at(k),headList.at(k+1),fData->getRead_meters(i)));

                     else
                         rollList.append(gen->AutoRoll(headList.at(k),fData->getRead_heading(i+1),fData->getRead_meters(i)));
                 }
             }
             else{
                 for(int k=0;k<latList.size();k++){
                     rollList.append(",0,");
                 }
             }

             float totalFeetDiff=QString(fData->getRead_totalFeet(i+1)).toFloat()-QString(fData->getRead_totalFeet(i)).toFloat();
             float totalFeetIn;

             float feetDiff=QString(fData->getRead_feet(i+1)).toFloat()-QString(fData->getRead_feet(i)).toFloat();
             float feetIn;

             float ktsDiff=QString(fData->getRead_kts(i+1)).toFloat()-QString(fData->getRead_kts(i)).toFloat();
             float ktsIn;

             float kmhDiff=QString(fData->getRead_vvi(i+1)).toFloat()-QString(fData->getRead_vvi(i)).toFloat();

             float kmhIn;

             for(int j=0;j<latList.size();j++){


                 timeIn=time1+((j)*scale)*time3Diff;
                 feetIn=QString(fData->getRead_feet(i)).toFloat()+((j)*scale)*feetDiff;
                 totalFeetIn=QString(fData->getRead_totalFeet(i)).toFloat()+((j)*scale)*totalFeetDiff;
                 ktsIn=QString(fData->getRead_kts(i)).toFloat()+((j)*scale)*ktsDiff;
                 kmhIn=QString(fData->getRead_vvi(i)).toFloat()+((j)*scale)*kmhDiff;

                 stream<<"\nDATA, "+QString::number(timeIn,'f',0)+",65.0,"+QString::number(longList.at(j),'f',4)+","+QString::number(latList.at(j),'f',4)+","+QString::number(totalFeetIn,'f',4)+", "+QString::number(feetIn,'f',4)+",0.0,0.0,0.0,"+gen_pitch+gen_roll+fData->getRead_heading(i)+", "+QString::number(ktsIn,'f',4)+","+QString::number(kmhIn,'f',4)+",0.0,0.0,0.0,0.0,0.0, "+"0.0,0.0,0.0,0.0,"+gen_gear+gen_gear+gen_gear+gen_gear+"0.1, 11010,10930,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,1.0,1.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,8.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0, 29.92,0.0,0.0,0.0,0.0,2.0,2.0,1.0,1.0,1.0,1.0,"+gen_power+gen_power+"1.2,1.2,1.2,1.2, 600.0,600.0,600.0,600.0, 5000.0,5000.0,5000.0,5000.0, 0.0,0.0,0.0,0.0, 0.0,0.0,0.0,0.0, 0.0,0.0,0.0,0.0";
                 stream<<"\nDATA, "+QString::number(timeIn,'f',0)+",65.0,"+QString::number(longList.at(j),'f',4)+","+QString::number(latList.at(j),'f',4)+","+QString::number(totalFeetIn,'f',4)+", "+QString::number(feetIn,'f',4)+",0.0,0.0,0.0,"+gen_pitch+gen_roll+fData->getRead_heading(i)+", "+QString::number(ktsIn,'f',4)+","+QString::number(kmhIn,'f',4)+",0.0,0.0,0.0,0.0,0.0, "+"0.0,0.0,0.0,0.0,"+gen_gear+gen_gear+gen_gear+gen_gear+"0.1, 11010,10930,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,1.0,1.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,8.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0, 29.92,0.0,0.0,0.0,0.0,2.0,2.0,1.0,1.0,1.0,1.0,"+gen_power+gen_power+"1.2,1.2,1.2,1.2, 600.0,600.0,600.0,600.0, 5000.0,5000.0,5000.0,5000.0, 0.0,0.0,0.0,0.0, 0.0,0.0,0.0,0.0, 0.0,0.0,0.0,0.0";

             }

         }
    latList.clear();
    longList.clear();
    timeList.clear();
    headList.clear();
    pitchList.clear();
    rollList.clear();

}

void Core::GenerateIf(int i){

    lon=fData->getRead_longitude(i);
    lat=fData->getRead_latitude(i);

    if((!(fData->size()-1==i))&&(conf->genPitch.contains("true"))){
        gen_pitch=planePitch->getPitch(fData->getRead_latitude(i),fData->getRead_longitude(i),fData->getRead_meters(i),fData->getRead_latitude(i+1),fData->getRead_longitude(i+1),fData->getRead_meters(i+1));
    }
    else{
        gen_pitch="0";
    }
    if((!(fData->size()-1==i))&& (conf->genGear.contains("true"))){
        gen_gear=gen->AutoGear(fData->getRead_meters(i));
    }
    else{
        gen_gear="1,";
    }
    if(((fData->size()-2>i))&& (conf->genPower.contains("true"))){
        gen_power=gen->AutoEngine(fData->getRead_kts(i),fData->getRead_totalFeet(i),fData->getRead_vvi(i+1));
    }
    else if(((fData->size()-1==i))&& (conf->genPower.contains("true"))){
        gen_power=gen->AutoEngine(fData->getRead_kts(i),fData->getRead_totalFeet(i),fData->getRead_vvi(i));
    }
    else{
        gen_power="100,100,100,100,";
    }
    if(((fData->size()-2>i))&& (conf->genRoll.contains("true"))){
        gen_roll=gen->AutoRoll(fData->getRead_heading(i),fData->getRead_heading(i+1),fData->getRead_meters(i));
    }
    else{
        gen_roll=",0,";
    }

}


void Core::GenerateToFixBugKml(int i){
    lonf=QString(fData->getRead_longitude(i)).toFloat();
    lonf2=QString(fData->getRead_longitude(i+1)).toFloat();
    latf=QString(fData->getRead_latitude(i)).toFloat();
    latf2=QString(fData->getRead_latitude(i+1)).toFloat();


    //timeDiff;
    date2=QDateTime::fromString(QString(fData->getRead_date(i+1)), "yyyy-MM-ddTHH:mm:ss");
    timeNow=date2.toTime_t();
    float timeDiff2=timeNow-timeBegin;

    GenerateToFixBug2(latf,lonf,latf2,lonf2,timeDiff,timeDiff2,i);
}


//Others
void Core::error(){
    QMessageBox msgBox;
    msgBox.setText("Sorry, something goes wrong. Not enough space on drive or save error. Try again!");
    msgBox.exec();
}
void Core::ClearMemoryRead(){
    fData->clearAll();

    totalFeetF=0;
    timeBegin=0;
    timeNow=0;
    timeDiff=0;

    date1.setTime_t(0);
    date2.setTime_t(0);

    txtKml=0;

    stream.flush();

    fileSave.close();

    loop.quit();
}
