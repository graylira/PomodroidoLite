#-------------------------------------------------
#
# Project created by QtCreator 2018-11-21T13:05:09
#
#-------------------------------------------------

QT += core gui
QT += charts
QT += datavisualization
QT += sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PomodroidoLite
TEMPLATE = app
RC_ICONS = pomodro.ico
# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        pomodroidolitemain.cpp \
    user/graphbars3d.cpp \
    settomatodialog.cpp \
    databaseoperate.cpp \
    dbquerydialog.cpp \
    aboutintrodialog.cpp \
    gadget/countdowntimer.cpp \
    createnotepaddialog.cpp \
    savenotepadinidialog.cpp \
    button.cpp \
    calculator.cpp

HEADERS += \
        pomodroidolitemain.h \
    user/graphbars3d.h \
    settomatodialog.h \
    parameter_config.h \
    databaseoperate.h \
    dbquerydialog.h \
    aboutintrodialog.h \
    gadget/countdowntimer.h \
    createnotepaddialog.h \
    savenotepadinidialog.h \
    button.h \
    calculator.h

FORMS += \
        pomodroidolitemain.ui \
    settomatodialog.ui \
    dbquerydialog.ui \
    aboutintrodialog.ui \
    gadget/countdowntimer.ui \
    createnotepaddialog.ui \
    savenotepadinidialog.ui

DISTFILES +=

RESOURCES += \
    pic.qrc
