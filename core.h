#ifndef Core_H
#define Core_H

#include <QMainWindow>
#include <QFile>
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>
#include <QDateTime>
#include <options.h>
#include <QProcess>
#include <pitchcalc.h>
#include <generate.h>
#include <QXmlStreamReader>
#include <QFileInfo>
#include <wait.h>
#include <info.h>
#include <QList>
#include <googleapi.h>
#include <flightplot.h>
#include <flightdata.h>


namespace Ui {
class MainWindow;
}

class Core : public QMainWindow
{
    Q_OBJECT

public:
    explicit Core(QWidget *parent = 0);
    ~Core();

    QString line;
    QString fileName;
    QString fileNameS;

    QFile fileRead;

    bool fileReadGood;


private:
    Ui::MainWindow *ui;
    QStringList read_api;

    QDateTime date1;
    QDateTime date2;

    QString lon;
    QString lat;

    QString gen_pitch;
    QString gen_gear;
    QString gen_power;
    QString gen_head;
    QString gen_roll;
    QString totalFeet;
    QString req;

    float lonf;
    float latf;
    float lonf2;
    float latf2;

    float fpm;
    float totalFeetF;
    float timeDiff;
    float feetMet;

    int timeBegin;
    int timeNow;

    int txtKml; // if 1 then txt elseif 2 then kml
    int kmlType; // if 1 fligtaware else if 2 sec kml



    QFile fileSave;
    QTextStream stream;
    QEventLoop loop;

    PitchCalc *planePitch;
    Generate *gen;
    Wait *loading;
    Info *about;
    Options *conf;
    GoogleApi *api;
    FlightPlot *plot;
    flightData* fData;

    void startSimulator();
    bool TxtFileToFdr(bool);
    bool KmlFileToFdr(bool);
    bool ReadFile();
    bool ReadFileKml(QString);
    void  closeEvent(QCloseEvent*);
    void GenerateToFixBug(int);
    void GenerateToFixBugKml(int);
    void GenerateToFixBug2(float,float,float,float,float,float,int);
    void GenerateIf(int);



private slots:
    void SimulatedMsgOpt();
    void ClearMemoryRead();
    void Simulate();
    void GenerateFDR();
    void on_actionExit_triggered();
    void on_actionOptions_triggered();
    void on_actionInfo_triggered();
    void error();

signals:
    void errorS();
};

#endif // Core_H
