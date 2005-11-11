TARGET = test1

QMAKE_CXXFLAGS_DEBUG += -pg -O2
QMAKE_LFLAGS_DEBUG += -pg

CONFIG += debug warn_on
CONFIG -= qt

INCLUDEPATH += ../../include

DEPENDPATH  += ../../include

SOURCES		= ./test1.cpp

LIBS += ../../lib/libnnfw.a
