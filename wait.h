#ifndef WAIT_H
#define WAIT_H

#include <QDialog>
#include <QDebug>

namespace Ui {
class Wait;
}

class Wait : public QDialog
{
    Q_OBJECT

public:
    explicit Wait(QWidget *parent = 0);
    ~Wait();

private:
    Ui::Wait *ui;

};

#endif // WAIT_H
