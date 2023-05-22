TEMPLATE        = lib
CONFIG         += plugin
QT             += widgets
INCLUDEPATH    += ../MT4CTP

CONFIG += c++11


TARGET          = $$qtLibraryTarget(shortalertexample)
DESTDIR         = ../MT4CTP/plugins/alert

CONFIG += install_ok  # Do not cargo-cult this!

SOURCES += \
    shortalertexample.cpp

HEADERS += \
    shortalertexample.h
