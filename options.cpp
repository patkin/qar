#include "options.h"
#include "ui_options.h"

Options::Options(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Options)
{
    ui->setupUi(this);
    //this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    connect(ui->pushButton,SIGNAL(clicked(bool)),this,SLOT(close()));
    connect(ui->pushButton_3,SIGNAL(clicked(bool)),this,SLOT(ChangeSource()));
    connect(ui->pushButton_2,SIGNAL(clicked(bool)),this,SLOT(SaveOptions()));
    connect(ui->pushButton_4,SIGNAL(clicked(bool)),this,SLOT(ChangeModel()));
    connect(ui->checkBox,SIGNAL(clicked(bool)),this,SLOT(ChangePitch()));
    connect(ui->checkBox_2,SIGNAL(clicked(bool)),this,SLOT(ChangeGear()));
    connect(ui->checkBox_3,SIGNAL(clicked(bool)),this,SLOT(ChangePower()));
    connect(ui->checkBox_4,SIGNAL(clicked(bool)),this,SLOT(ChangeRoll()));
}

Options::~Options()
{
    delete ui;
}
 void Options::ReadOptions(){

      QFile fileOptions(QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation).at(0)+"/QAR-PlaneOptions/config.ini");
      if(! fileOptions.exists()){
          if(! QDir(QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation).at(0)+"/QAR-PlaneOptions").exists()){
              QDir(QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation).at(0)).mkdir("QAR-PlaneOptions");
          }
        fileOptions.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream streamCreate(&fileOptions);
        streamCreate<<"source: not_chosen\n";
        streamCreate<<"model: Aircraft/General Aviation/Cessna 172SP/Cessna_172SP.acf\n";
        streamCreate<<"pitch: false\n";
        streamCreate<<"gear: false\n";
        streamCreate<<"engine: false\n";
        streamCreate<<"roll: false\n";
        fileOptions.close();
      }
        if (!fileOptions.open(QIODevice::ReadOnly | QIODevice::Text))
            return;
        QString lineOptions;
        QRegExp sep("\\s+");
        while (!fileOptions.atEnd()) {
            lineOptions = fileOptions.readLine();
            if((lineOptions.contains("source:"))){
            source=lineOptions.section(sep,1).simplified();
            QRegExp sepPath("/");
            sourceFolder=source.section(sepPath,0,-2);
            }
            if((lineOptions.contains("model:"))){
            model=lineOptions.section(sep,1).simplified();
            }
            if((lineOptions.contains("pitch:"))){
            genPitch=lineOptions.section(sep,1).simplified();
            }
            if((lineOptions.contains("gear:"))){
            genGear=lineOptions.section(sep,1).simplified();
            }
            if((lineOptions.contains("engine:"))){
            genPower=lineOptions.section(sep,1).simplified();
            }
            if((lineOptions.contains("roll:"))){
            genRoll=lineOptions.section(sep,1).simplified();
            }
        }

        fileOptions.close();
        ui->label_2->setText(source);
        ui->label_4->setText(model);
        if(genPitch=="false"){
            ui->checkBox->setChecked(false);
        }
        else{
            ui->checkBox->setChecked(true);
        }
        if(genGear=="false"){
            ui->checkBox_2->setChecked(false);
        }
        else{
            ui->checkBox_2->setChecked(true);
        }
        if(genPower=="false"){
            ui->checkBox_3->setChecked(false);
        }
        else{
            ui->checkBox_3->setChecked(true);
        }
        if(genRoll=="false"){
            ui->checkBox_4->setChecked(false);
        }
        else{
            ui->checkBox_4->setChecked(true);
        }

        if(source.contains("not_chosen")||source.isEmpty()){
            badOptions=true;

        }
        else{
            badOptions=false;
        }
 }

 void Options::ChangeSource(){
newSource=QFileDialog::getOpenFileName(this,tr("Open..."), "/home/", tr("X-Plane 10(X-Plane.exe X-Plane-32bit.exe))"));
ui->label_2->setText(newSource);
 }

 void Options::SaveOptions(){
     if(((source!=newSource) && !(newSource.isEmpty())) || ((model!=newModel) && !(newModel.isEmpty())) || (genPitch!=newGenPitch) || (genGear!=newGear) || (genPower!=newPower)|| (genRoll!=newRoll)){
         //qDebug()<<genPower;
         QFile fileSaveOpt(QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation).at(0)+"/QAR-PlaneOptions/config.ini");
         fileSaveOpt.open(QIODevice::WriteOnly | QIODevice::Text);
         QTextStream streamOptions(&fileSaveOpt);

            if((source!=newSource) && !(newSource.isEmpty())){
                streamOptions<<"source: "+QString(newSource)+"\n";
                badOptions=false;

            }
            else {
                streamOptions<<"source: "+QString(source)+"\n";
            }

            if((model!=newModel) && !(newModel.isEmpty())){
                streamOptions<<"model: "+QString(newModel)+"\n";

            }
            else {
                streamOptions<<"model: "+QString(model)+"\n";
            }
            if((genPitch!=newGenPitch)&&(!newGenPitch.isEmpty())){
                streamOptions<<"pitch: "+QString(newGenPitch)+"\n";

            }
            else {
                streamOptions<<"pitch: "+QString(genPitch)+"\n";
            }
            if((genGear!=newGear)&&(!newGear.isEmpty())){
                streamOptions<<"gear: "+QString(newGear)+"\n";

            }
            else {
                streamOptions<<"gear: "+QString(genGear)+"\n";
            }
            if((genPower!=newPower)&&(!newPower.isEmpty())){
                streamOptions<<"engine: "+QString(newPower)+"\n";

            }
            else {
                streamOptions<<"engine: "+QString(genPower)+"\n";
            }
            if((genRoll!=newRoll)&&(!newRoll.isEmpty())){
                streamOptions<<"roll: "+QString(newRoll)+"\n";

            }
            else {
                streamOptions<<"roll: "+QString(genRoll)+"\n";
            }

      fileSaveOpt.close();
     }
     this->close();
     ReadOptions();
 }

 void Options::ChangeModel(){
     if((source.contains("not_chosen"))||(source.isEmpty())){
         QMessageBox msgBox;
         msgBox.setText("First choose X-Plane exe");
         msgBox.exec();
         return;
     }
     else{
         QString path=sourceFolder+"/Aircraft/";
         //qDebug()<<path;
         newModel=QFileDialog::getOpenFileName(this,tr("Open..."), path, tr("Airplane model(*.acf))"));
         newModel.replace(sourceFolder+"/","");
         ui->label_4->setText(newModel);
     }

 }

 void Options::ChangePitch(){
     if(ui->checkBox->isChecked()){
         newGenPitch="true";
     }
     else{
         newGenPitch="false";
     }
 }

 void Options::ChangeGear(){
     if(ui->checkBox_2->isChecked()){
         newGear="true";
     }
     else{
         newGear="false";
     }
}

void Options::ChangePower(){
    if(ui->checkBox_3->isChecked()){
        newPower="true";
    }
    else{
        newPower="false";
    }
}

void Options::ChangeRoll(){
    if(ui->checkBox_4->isChecked()){
        newRoll="true";
    }
    else{
        newRoll="false";
    }
}
