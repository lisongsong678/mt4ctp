TEMPLATE        = lib
CONFIG         += plugin
QT             += widgets
INCLUDEPATH    += ../MT4CTP
CONFIG += debug_and_release
CONFIG += c++11

DEFINES += QCUSTOMPLOT_USE_LIBRARY

HEADERS += \
    maplugin.h

SOURCES += \
    maplugin.cpp

TARGET          = $$qtLibraryTarget(ma)
DESTDIR         = ../MT4CTP/plugins/indicator

CONFIG(debug, debug|release) {
    win32: LIBS += -L$$PWD/../tulip/ -ltulipd
} else {
    win32: LIBS += -L$$PWD/../tulip/ -ltulip
}

INCLUDEPATH += $$PWD/../tulip
DEPENDPATH += $$PWD/../tulip

CONFIG += install_ok  # Do not cargo-cult this!

CONFIG(debug, debug|release) {
    win32: LIBS += -L$$PWD/../QCustomPlot/ -lqcustomplotd2
} else {
    win32: LIBS += -L$$PWD/../QCustomPlot/ -lqcustomplot2
}

INCLUDEPATH += $$PWD/../QCustomPlot
DEPENDPATH += $$PWD/../QCustomPlot

unix:!macx: QMAKE_CXXFLAGS += -Wunused-parameter
win32-msvc* {
    QMAKE_LFLAGS_DEBUG += /INCREMENTAL:NO
    QMAKE_CXXFLAGS *=  /wd"4100"
    contains (QMAKE_CXXFLAGS_WARN_ON, -w34100) : QMAKE_CXXFLAGS_WARN_ON -= -w34100
}


