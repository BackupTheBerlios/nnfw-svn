TEMPLATE = lib
TARGET   = ./lib/nnfw
VERSION  = 0.3.3

#unix:QMAKE_CXXFLAGS_DEBUG += -pg
#unix:QMAKE_LFLAGS_DEBUG += -pg

CONFIG += debug staticlib warn_on

contains( CONFIG, debug ) {
	DEFINES += VERBOSE
    unix:QMAKE_CXXFLAGS -= -O2
}

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
              ./include/supervisedlearning.h

SOURCES		= ./src/simplecluster.cpp \
              ./src/fakecluster.cpp \
              ./src/clusterupdater.cpp \
              ./src/libclusterupdaters.cpp \
              ./src/matrixlinker.cpp \
              ./src/copylinker.cpp \
			  ./src/neuralnet.cpp \
			  ./src/messages.cpp \
              ./src/random.cpp \
              ./src/types.cpp \
              ./src/supervisedlearning.cpp

unix:LIBS += -L/usr/local/lib -lgsl -lgslcblas -lm

target.path = /usr/local/lib
includes.path = /usr/local/include/nnfw
includes.files = ./include/*.h
INSTALLS   += target includes

OBJECTS_DIR =	./.obj
