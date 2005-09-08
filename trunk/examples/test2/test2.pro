TARGET = test2

QMAKE_CXXFLAGS_DEBUG += -pg -O2
QMAKE_LFLAGS_DEBUG += -pg

CONFIG += debug warn_on

INCLUDEPATH += ../../include

DEPENDPATH  += ../../include

SOURCES		= ./test2.cpp

LIBS += ../../lib/libnnfw.a -lgsl -lgslcblas
