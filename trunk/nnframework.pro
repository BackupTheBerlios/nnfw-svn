TEMPLATE = lib
TARGET   = ./lib/nnfw
VERSION  = 0.1

CONFIG += debug staticlib rtti warn_on
CONFIG -= qt

contains( CONFIG, debug ) {
    unix:QMAKE_CXXFLAGS -= -O2
    #unix:QMAKE_CXXFLAGS_DEBUG += -pg
    #unix:QMAKE_LFLAGS_DEBUG += -pg
}

#LIBS += -lgsl -lgslcblas -lm

INCLUDEPATH += ./include

DEPENDPATH  += ./include

HEADERS		= ./include/nnfw.h \
			  ./include/types.h \
              ./include/updatable.h \
			  ./include/cluster.h \
              ./include/simplecluster.h \
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

target.path = /usr/local/lib
includes.path = /usr/local/include/nnfw
includes.files = ./include/*.h
INSTALLS   += target includes
