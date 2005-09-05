TARGET = nnfw_vs_mint

QMAKE_CXXFLAGS_DEBUG += -pg -O2 -ffast-math
QMAKE_LFLAGS_DEBUG += -pg

CONFIG += release warn_on

INCLUDEPATH += ../../include

DEPENDPATH  += ../../include

SOURCES		= ./nnfw_vs_mint.cpp

LIBS += ../../lib/libnnfw.a ./libmint.a -lgsl -lgslcblas -lm
