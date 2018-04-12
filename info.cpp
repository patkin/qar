#include "info.h"
#include "ui_info.h"

Info::Info(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Info)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
}

Info::~Info()
{
    delete ui;
}

void Info::on_pushButton_clicked(bool checked)
{
    this->close();
}

void Info::on_pushButton_clicked()
{
    this->close();
}
