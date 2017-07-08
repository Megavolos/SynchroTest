#-------------------------------------------------
#
# Project created by QtCreator 2017-05-11T14:04:57
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SynchroTest
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    comportsettingsdialog.cpp \
    writetofiledialog.cpp \
    port.cpp \
    diffgraphwindow.cpp \
    diagramsettingswindow.cpp \
    sensor.cpp

HEADERS  += mainwindow.h \
    comportsettingsdialog.h \
    writetofiledialog.h \
    port.h \
    diffgraphwindow.h \
    diagramsettingswindow.h \
    sensor.h

FORMS    += mainwindow.ui \
    comportsettingsdialog.ui \
    writetofiledialog.ui \
    diffgraphwindow.ui \
    diagramsettingswindow.ui
win32:CONFIG += qwt
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../usr/local/qwt-6.1.3/lib/debug/ -lqwt
else:unix:!macx: LIBS += -L$$PWD/../../../usr/local/qwt-6.1.3/lib/ -lqwt

INCLUDEPATH += $$PWD/../../../usr/local/qwt-6.1.3/include
DEPENDPATH += $$PWD/../../../usr/local/qwt-6.1.3/include
