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
    signal.cpp

HEADERS  += mainwindow.h \
    comportsettingsdialog.h \
    writetofiledialog.h \
    port.h \
    signal.h

FORMS    += mainwindow.ui \
    comportsettingsdialog.ui \
    writetofiledialog.ui
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../usr/local/qwt-6.1.3/lib/release/ -lqwt
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../usr/local/qwt-6.1.3/lib/debug/ -lqwt
else:unix:!macx: LIBS += -L$$PWD/../../../usr/local/qwt-6.1.3/lib/ -lqwt

INCLUDEPATH += $$PWD/../../../usr/local/qwt-6.1.3/include
DEPENDPATH += $$PWD/../../../usr/local/qwt-6.1.3/include
