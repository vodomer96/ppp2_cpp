TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -std=c++1z --pedantic -O0

DESTDIR = ../bin

SOURCES += main.cpp

HEADERS += \
    File_handle.hpp
