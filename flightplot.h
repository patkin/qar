#ifndef FLIGHTPLOT_H
#define FLIGHTPLOT_H

#include <QDialog>
#include <qcustomplot.h>
#include<QString>
#include <QDateTime>
#include <QMessageBox>
#include <QListWidgetItem>
#include <QDebug>
#include <QColor>
#include <flightdata.h>

namespace Ui {
class FlightPlot;
}

class FlightPlot : public QDialog
{
    Q_OBJECT

public:
    explicit FlightPlot(QWidget *parent = 0);
    void setPlots(int type);
    ~FlightPlot();
    bool checkStatus();
    QString getMarks();
    int getStart();
    int getStop();

signals:

    void Done();

private:
    Ui::FlightPlot *ui;
    QString marks;
    bool SetMarks();
    //bool Done();
    QList <int> timeM;
    QStringList marksList;
    bool status;
    void clearAll();
    int startTime,stopTime;
    flightData* fData;

private slots:
    void on_pushButton_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_2_clicked();


    void on_spinBox_3_valueChanged(int arg1);
    void on_spinBox_2_valueChanged(int arg1);
};

#endif // FLIGHTPLOT_H
