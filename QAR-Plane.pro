
QT       += core gui\
            network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = QAR-Plane
TEMPLATE = app

win32:RC_ICONS += qrplane.ico

SOURCES +=\
    options.cpp \
    pitchcalc.cpp \
    generate.cpp \
    wait.cpp \
    info.cpp \
    googleapi.cpp \
    qcustomplot.cpp \
    flightplot.cpp \
    flightdata.cpp \
    main.cpp \
    core.cpp

HEADERS  += \
    options.h \
    pitchcalc.h \
    generate.h \
    wait.h \
    info.h \
    googleapi.h \
    qcustomplot.h \
    flightplot.h \
    flightdata.h \
    core.h

FORMS    += mainwindow.ui \
    options.ui \
    wait.ui \
    info.ui \
    flightplot.ui

RESOURCES += \
    appfiles.qrc
