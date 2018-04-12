#include "core.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Core run;
    run.show();

    return a.exec();
}
