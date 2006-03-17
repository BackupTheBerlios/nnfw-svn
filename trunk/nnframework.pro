TEMPLATE = lib
TARGET   = ./lib/nnfw
VERSION  = 0.2

CONFIG += release staticlib rtti warn_on
CONFIG -= debug
CONFIG -= qt

!isEmpty( DEBUG ) {
    CONFIG -= release
    CONFIG += debug
}

contains( CONFIG, debug ) {
    unix:QMAKE_CXXFLAGS -= -O2
    unix:QMAKE_CXXFLAGS += -O0
    #unix:QMAKE_CXXFLAGS_DEBUG += -pg
    #unix:QMAKE_LFLAGS_DEBUG += -pg
}

#### Definition for creating library under linux
linux:LIBS += -L/usr/lib -lgsl -lgslcblas -lm

#### Definition for creating library under windows
#########  NOThing

INCLUDEPATH += ./include

DEPENDPATH  += ./include

HEADERS		= ./include/nnfw.h \
			  ./include/types.h \
              ./include/updatable.h \
			  ./include/cluster.h \
              ./include/simplecluster.h \
              ./include/biasedcluster.h \
              ./include/fakecluster.h \
			  ./include/clusterupdater.h \
              ./include/derivableclusterupdater.h \
			  ./include/libclusterupdaters.h \
			  ./include/linker.h \
              ./include/copylinker.h \
              ./include/matrixlinker.h \
			  ./include/neuralnet.h \
			  ./include/utils.h \
			  ./include/messages.h \
			  ./include/random.h \
              ./include/blockslearning.h \
              ./include/learningnetwork.h

SOURCES		= ./src/simplecluster.cpp \
              ./src/biasedcluster.cpp \
              ./src/fakecluster.cpp \
              ./src/clusterupdater.cpp \
              ./src/libclusterupdaters.cpp \
              ./src/matrixlinker.cpp \
              ./src/copylinker.cpp \
			  ./src/neuralnet.cpp \
              ./src/utils.cpp \
			  ./src/messages.cpp \
              ./src/random.cpp \
              ./src/types.cpp \
              ./src/blockslearning.cpp \
              ./src/learningnetwork.cpp

OBJECTS_DIR =   ./.obj

target.path = /usr2/local/lib
includes.path = /usr2/local/include/nnfw
includes.files = ./include/*.h
INSTALLS   += target includes
