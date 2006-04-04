TEMPLATE = lib
TARGET   = ./lib/nnfw
VERSION  = 0.2.1

CONFIG += release staticlib rtti warn_on
CONFIG -= debug
CONFIG -= qt
unix:QMAKE_CXXFLAGS -= -O2

!isEmpty( DEBUG ) {
    CONFIG -= release
    CONFIG += debug
}

contains( CONFIG, debug ) {
    unix:QMAKE_CXXFLAGS += -O0
    DEFINES += NNFW_DEBUG
}
contains( CONFIG, release ) {
    unix:QMAKE_CXXFLAGS += -O3
    unix:QMAKE_CXXFLAGS += -funroll-loops
    unix:QMAKE_CXXFLAGS -= -O2
}

#### Definition for creating library under linux
unix:LIBS += -L/usr/lib -lgsl -lgslcblas -lm

#### Intel Math Library configuration ----- UNIX MACHINES ONLY
#### Controllo se le MKL sono installate
####  - rpm -qa | grep mkl   =>  intel-mkl-8.0.2p-4
####  - rpm -ql intel-mkl-8.0.2p-4 | grep include$   => /usr2/intel/mkl/8.0.2/include
unix {
    message( Check if Intel Math Kernel Library is installed ... )
    INTEL_MKL=$$system(rpm -qa | grep mkl)
    MKL_PATH=$$system(rpm -ql $$INTEL_MKL | grep include$)
    exists( $$MKL_PATH/mkl.h ) {
        message( found ... configuring NNFW with Intel MKL )
        INCLUDEPATH += $$MKL_PATH
        DEFINES += NNFW_USE_MKL
    } else {
        message( not found ... configuring NNFW without MKL )
    }
}

#### Definition for creating library under windows
#########  NOThing

INCLUDEPATH += ./include

DEPENDPATH  += ./include

HEADERS		= ./include/nnfw.h \
			  ./include/types.h \
              ./include/realvector.h \
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

SOURCES		= ./src/cluster.cpp \
              ./src/simplecluster.cpp \
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
              ./src/realvec.cpp \
              ./src/blockslearning.cpp \
              ./src/learningnetwork.cpp

OBJECTS_DIR =   ./.obj

target.path = /usr2/local/lib
includes.path = /usr2/local/include/nnfw
includes.files = ./include/*.h
INSTALLS   += target includes
