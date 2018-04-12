#ifndef OPTIONS_H
#define OPTIONS_H

#include <QDialog>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QDebug>
#include <QStandardPaths>
#include <QMessageBox>


namespace Ui {
class Options;
}

class Options : public QDialog
{
    Q_OBJECT

public:
    explicit Options(QWidget *parent = 0);
    ~Options();

    QString source;
    QString model;
    QString sourceFolder;
    QString genPitch;
    QString genGear;
    QString genPower;
    QString genRoll;
    bool badOptions;


private:
    Ui::Options *ui;
    QString newSource;
    QString newModel;
    QString newGenPitch;
    QString newGear;
    QString newPower;
    QString newRoll;

private slots:
    void ChangeSource();
    void SaveOptions();
    void ChangeModel();
    void ChangePitch();
    void ChangeGear();
    void ChangePower();
    void ChangeRoll();

public slots:
    void ReadOptions();

};

#endif // OPTIONS_H
