TEMPLATE        = lib
CONFIG         += plugin
QT             += widgets
INCLUDEPATH    += ../MT4CTP
CONFIG += debug_and_release
CONFIG += c++11

HEADERS += \
    macrossstrategy.h

SOURCES += \
    macrossstrategy.cpp

TARGET          = $$qtLibraryTarget(macrossstrategy)
DESTDIR         = ../MT4CTP/plugins/strategy
##策略计算时需要指标库就加上去，此处不需要，可以不加
CONFIG(debug, debug|release) {
    win32: LIBS += -L$$PWD/../tulip/ -ltulipd
} else {
    win32: LIBS += -L$$PWD/../tulip/ -ltulip
}

INCLUDEPATH += $$PWD/../tulip
DEPENDPATH += $$PWD/../tulip

CONFIG += install_ok  # Do not cargo-cult this!

unix:!macx: QMAKE_CXXFLAGS += -Wunused-parameter
win32-msvc* {
    QMAKE_LFLAGS_DEBUG += /INCREMENTAL:NO
    QMAKE_CXXFLAGS *=  /wd"4100"
    contains (QMAKE_CXXFLAGS_WARN_ON, -w34100) : QMAKE_CXXFLAGS_WARN_ON -= -w34100
}
