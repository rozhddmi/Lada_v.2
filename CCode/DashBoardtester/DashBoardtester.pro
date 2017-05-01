#-------------------------------------------------
#
# Project created by QtCreator 2016-08-29T21:59:13
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DashBoardtester
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

win32:CONFIG(release, debug|release): LIBS += -LC:\Users\rozhddmi\CVUTrep\dashboard_simulator\Gauges_stand_along\Release -lDashBoardDLL
else:win32:CONFIG(debug, debug|release): LIBS += -LC:\Users\rozhddmi\CVUTrep\dashboard_simulator\Gauges_stand_along\Debug -lDashBoardDLL

INCLUDEPATH += C:\Users\rozhddmi\CVUTrep\dashboard_simulator\Gauges_stand_along\Release

INCLUDEPATH += C:\Users\rozhddmi\CVUTrep\dashboard_simulator\DashBoardDLL\DashBoardDLL

DEPENDPATH += C:\Users\rozhddmi\CVUTrep\dashboard_simulator\Gauges_stand_along\Release

DEPENDPATH += C:\Users\rozhddmi\CVUTrep\dashboard_simulator\Gauges_stand_along\Debug
