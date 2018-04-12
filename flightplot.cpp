#include "flightplot.h"
#include "ui_flightplot.h"

FlightPlot::FlightPlot(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FlightPlot)
{
    ui->setupUi(this);
    flightData* fData = flightData::getInstance();

}

FlightPlot::~FlightPlot()
{
    delete ui;
}


void FlightPlot::setPlots(int type){ // type 1 = txt file, 2 = kml google, 3 = kml flightaware

    clearAll();
    int size = fData->size(); // numbers of rows

    //Time options, creating axis to plot

    QList <double> timeSec;
    double maxFeet=0;

    switch(type){
    case 1:
    {
        QDateTime date1=QDateTime::fromString(QString(fData->getRead_date(0))+"T"+QString(fData->getRead_time(0)), "yyyy/MM/ddTHH:mm:ss");
        double timeBegin=date1.toTime_t();
        double timeNow=0;
        QDateTime date2;
        for(int i=0;i<size;i++){
            date2=QDateTime::fromString(QString(fData->getRead_date(i))+"T"+QString(fData->getRead_time(i)), "yyyy/MM/ddTHH:mm:ss");
            timeNow=date2.toTime_t();
            timeSec.append(timeNow-timeBegin);
        }
        break;
    }
    case 2:
    {
        QDateTime date1=QDateTime::fromString(QString(fData->getRead_date(0)), "yyyy-MM-ddTHH:mm:ss");
        double timeBegin=date1.toTime_t();
        double timeNow=0;
        QDateTime date2;;
        for(int i=0;i<size;i++){
            date2=QDateTime::fromString(QString(fData->getRead_date(i)), "yyyy-MM-ddTHH:mm:ss");
            timeNow=date2.toTime_t();
            timeSec.append(timeNow-timeBegin);
        }
        break;
    }
    case 3:
    {
        QDateTime date1=QDateTime::fromString("2000-01-01T"+QString(fData->getRead_date(0)), "yyyy-MM-ddTHH:mm:ss");
        double timeBegin=date1.toTime_t();
        double timeNow=0;
        QDateTime date2;
        for(int i=0;i<size;i++){
            date2=QDateTime::fromString("2000-01-01T"+QString(fData->getRead_date(i)), "yyyy-MM-ddTHH:mm:ss");
            timeNow=date2.toTime_t();
            if(timeNow>=timeBegin)
            timeSec.append(timeNow-timeBegin);
            else{
                date2=QDateTime::fromString("2000-01-02T"+QString(fData->getRead_date(i)), "yyyy-MM-ddTHH:mm:ss");
                timeNow=date2.toTime_t();
                timeSec.append(timeNow-timeBegin);
            }
        }
            break;
    }

    default:
                qDebug()<<"Bad value in flightplot -> setPlots parametr";
                break;
    }

    //PLOT 1///////////////////////////////////////////////////////////////////////

    QVector<double> y(size), x(size);
    for(int i=0;i<size;i++){
       x[i]=timeSec.at(i);
       y[i]=QString(fData->getRead_feet(i)).toDouble();
       if(QString(fData->getRead_feet(i)).toDouble()>maxFeet)
           maxFeet=QString(fData->getRead_feet(i)).toDouble();
    }

    ui->spinBox->setMaximum((int)x[size-1]);
    ui->spinBox_2->setMaximum((int)x[size-1]);
    ui->spinBox_3->setMaximum(((int)x[size-1])-1);
    ui->spinBox_2->setValue((int)x[size-1]);

    ui->label->hide();
    ui->plot1->show();

    ui->plot1->addGraph();
    ui->plot1->graph(0)->setData(x, y);
    // labels:
    ui->plot1->xAxis->setLabel("time [sec]");
    ui->plot1->yAxis->setLabel("absolute altitude [feet]");
    // axes ranges:
    ui->plot1->xAxis->setRange(0, x[timeSec.size()-1]);
    ui->plot1->yAxis->setRange(0, maxFeet+200);

    ui->plot1->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, QPen(Qt::black, 1), QBrush(Qt::white), 3));
    ui->plot1->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes |
                                     QCP::iSelectLegend | QCP::iSelectPlottables);
    ui->plot1->replot();


    //PLOT 2//////////////////////////////////////////////////////////////////////////////
    double maxTotalFeet=0;

    QVector<double> z(size);
    for(int i=0;i<size;i++){
       z[i]=QString(fData->getRead_totalFeet(i)).toDouble();
       if(QString(fData->getRead_totalFeet(i)).toDouble()>maxTotalFeet)
           maxTotalFeet=QString(fData->getRead_totalFeet(i)).toDouble();
    }
    ui->label_2->setText("Data generated");
    ui->label_2->show();
    ui->plot2->show();

    ui->plot2->addGraph();
    ui->plot2->graph(0)->setData(x, z);
    // labels:
    ui->plot2->xAxis->setLabel("time [sec]");
    ui->plot2->yAxis->setLabel("true altitude [feet]");
    // axes ranges:
    ui->plot1->xAxis->setRange(0, x[timeSec.size()-1]);
    ui->plot2->yAxis->setRange(0, maxTotalFeet+500);

    ui->plot2->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, QPen(Qt::black, 1), QBrush(Qt::white), 3));
    ui->plot2->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes |
                                     QCP::iSelectLegend | QCP::iSelectPlottables);
    ui->plot2->replot();

    //PLOT3///////////////////////////////////////////////////////////////////////////////////////


    QVector<double> a(size);
    for(int i=0;i<size;i++){
       a[i]=QString(fData->getRead_heading(i)).toDouble();

    }


    if(type>1)
        ui->label_3->setText("Data generated");
    else
        ui->label_3->hide();

    ui->plot3->show();

    ui->plot3->addGraph();
    ui->plot3->graph(0)->setData(x, a);
    // labels:
    ui->plot3->xAxis->setLabel("time [sec]");
    ui->plot3->yAxis->setLabel("heading [degree]");
    // axes ranges:
    ui->plot1->xAxis->setRange(0, x[timeSec.size()-1]);
    ui->plot3->yAxis->setRange(0, 360);

    ui->plot3->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, QPen(Qt::black, 1), QBrush(Qt::white), 3));
    ui->plot3->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes |
                                     QCP::iSelectLegend | QCP::iSelectPlottables);
    ui->plot3->replot();

    //PLOT4/////////////////////////////////////////////////////////////////////////////////////////////

    double maxKts=0;

    QVector<double> b(size);
    for(int i=0;i<size;i++){
       b[i]=QString(fData->getRead_kts(i)).toDouble();
       if(QString(fData->getRead_kts(i)).toDouble()>maxKts)
           maxKts=QString(fData->getRead_kts(i)).toDouble();
    }

    if(type>1)
         ui->label_4->setText("Data generated");
    else
        ui->label_4->hide();

    ui->plot4->show();

    ui->plot4->addGraph();
    ui->plot4->graph(0)->setData(x, b);
    // labels:
    ui->plot4->xAxis->setLabel("time [sec]");
    ui->plot4->yAxis->setLabel("indicated air speed [knots]");
    // axes ranges:
    ui->plot1->xAxis->setRange(0, x[timeSec.size()-1]);
    ui->plot4->yAxis->setRange(0, maxKts+100);

    ui->plot4->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, QPen(Qt::black, 1), QBrush(Qt::white), 3));
    ui->plot4->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes |
                                     QCP::iSelectLegend | QCP::iSelectPlottables);
    ui->plot4->replot();

    //PLOT5////////////////////////////////////////////////////////////////////////////////////////////////

    double maxVvi=0;
    double minVvi=0;

    QVector<double> c(size);
    for(int i=0;i<size;i++){
       c[i]=QString(fData->getRead_vvi(i)).toDouble();
       if(QString(fData->getRead_vvi(i)).toDouble()>maxVvi)
           maxVvi=QString(fData->getRead_vvi(i)).toDouble();
       if(QString(fData->getRead_vvi(i)).toDouble()<minVvi)
           minVvi=QString(fData->getRead_vvi(i)).toDouble();

    }

    ui->label_5->setText("Data generated");
    ui->label_5->show();
    ui->plot5->show();

    ui->plot5->addGraph();
    ui->plot5->graph(0)->setData(x, c);
    // labels:
    ui->plot5->xAxis->setLabel("time [sec]");
    ui->plot5->yAxis->setLabel("indicated vertical speed [feet/min]");
    // axes ranges:
    ui->plot1->xAxis->setRange(0, x[timeSec.size()-1]);
    ui->plot5->yAxis->setRange(minVvi-50, maxVvi+50);

    ui->plot5->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, QPen(Qt::black, 1), QBrush(Qt::white), 3));
    ui->plot5->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes |
                                     QCP::iSelectLegend | QCP::iSelectPlottables);
    ui->plot5->replot();

    this->show();


}



//Controls

void FlightPlot::on_spinBox_3_valueChanged(int arg1)
{
   ui->spinBox_2->setMinimum(arg1+1);
   ui->spinBox->setMinimum(arg1+1);

   for(int i=0;i<ui->listWidget->count();i++){
       int temp=QString(ui->listWidget->item(i)->text()).section(',',0,0).toInt();
       if(temp<ui->spinBox_3->value())
           ui->listWidget->item(i)->setBackgroundColor(QColor(255,0,0));
       else
           ui->listWidget->item(i)->setBackgroundColor(QColor(255,255,255));
   }
}

void FlightPlot::on_spinBox_2_valueChanged(int arg1)
{
    ui->spinBox->setMaximum(arg1);
    ui->spinBox_3->setMaximum(arg1-1);

    for(int i=0;i<ui->listWidget->count();i++){
        int temp=QString(ui->listWidget->item(i)->text()).section(',',0,0).toInt();
        if(temp>ui->spinBox_2->value())
            ui->listWidget->item(i)->setBackgroundColor(QColor(255,0,0));
        else
            ui->listWidget->item(i)->setBackgroundColor(QColor(255,255,255));
    }

}

void FlightPlot::on_pushButton_clicked()
{
    if(!ui->lineEdit->text().isEmpty() && timeM.indexOf(ui->spinBox->value())==-1){
        QString temp=QString(ui->lineEdit->text().left(100));
        temp=temp.replace(","," ");
        ui->listWidget->addItem(QString::number(ui->spinBox->value())+", "+temp);
        timeM.append(ui->spinBox->value());
        marksList.append(QString::number(ui->spinBox->value())+", "+temp);
        ui->spinBox->setValue(1);
        ui->lineEdit->setText("");


    }
    else if(ui->lineEdit->text().isEmpty()){
        QMessageBox msgBox;
        msgBox.setText("Error, empty text line");
        msgBox.exec();
    }
    else if(timeM.indexOf(ui->spinBox->value())>-1){
        QMessageBox msgBox;
        msgBox.setText("Time error, choose another value");
        msgBox.exec();
    }
    else{
        QMessageBox msgBox;
        msgBox.setText("Error");
        msgBox.exec();
    }

}

void FlightPlot::on_pushButton_3_clicked()
{
    QString temp=ui->listWidget->currentItem()->text();
    temp=temp.section(',',0,0);
    timeM.removeAt(timeM.indexOf(temp.toInt()));
    marksList.removeAt(marksList.indexOf(ui->listWidget->currentItem()->text()));
    ui->listWidget->takeItem(ui->listWidget->currentRow());

}

void FlightPlot::on_pushButton_4_clicked()
{
    QRegExp sep(",");
    QString temp=QString(ui->listWidget->currentItem()->text()).simplified();
    ui->spinBox->setValue(QString(temp.section(sep,0,0)).toInt());
    ui->lineEdit->setText(temp.section(sep,1));
    QString temp2=ui->listWidget->currentItem()->text();
    temp=temp2.section(',',0,0);
    timeM.removeAt(timeM.indexOf(temp.toInt()));
    marksList.removeAt(marksList.indexOf(ui->listWidget->currentItem()->text()));
    ui->listWidget->takeItem(ui->listWidget->currentRow());

}

void FlightPlot::on_pushButton_2_clicked()
{
    startTime=ui->spinBox_3->value();
    stopTime=ui->spinBox_2->value();
    marks="";
    if(!marksList.isEmpty()){
        for(int i=0;i<marksList.size();i++){
            int tempMarkTime=QString(marksList.at(i)).section(',',0,0).toInt();
            //qDebug()<<tempMarkTime;
            if(!(tempMarkTime<startTime) && !(tempMarkTime>stopTime))
            marks+="\nMARK, "+marksList.at(i)+",";
        }
    }
    this->close();
    status=true;
    emit Done();
}


QString FlightPlot::getMarks(){
    return marks;
}

bool FlightPlot::checkStatus(){
    return status;
}

void FlightPlot::clearAll(){
    marks="";
    marksList.clear();
    timeM.clear();
    status=false;
    ui->listWidget->clear();
}

int FlightPlot::getStart(){
    return startTime;
}

int FlightPlot::getStop(){
    return stopTime;
}
