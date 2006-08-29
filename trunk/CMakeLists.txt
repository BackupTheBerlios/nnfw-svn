
PROJECT( NNFW )

### Configure of Library
### a string variable that contains configuration keys separated by with spaces:
### - double     <= use double precision for Real number
### - debug      <= compile with debug flags activated
### - gsl        <= link against GSL library
### - mkl        <= link against MKL library
### Default (empty) means that it will be compiled optimized without debug, GSL and MKL with single precision
SET( NNFW_CONFIG "" CACHE STRING "NNFW Generation Configuration" )

### Setting Compiler g++ for linux machines
IF (UNIX)
    MESSAGE( "-- Setting compiler for Linux" )
    SET( CMAKE_CXX_COMPILER "g++" )
    SET( CMAKE_CXX_FLAGS " -pipe -fno-strict-aliasing -Wall -W " )
    ### --- debug check
    IF ( NNFW_CONFIG MATCHES debug )
        ## Debug version
        SET( CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} " -g -O0 " )
    ELSE ( NNFW_CONFIG MATCHES debug )
        ## Optimized version
        SET( CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} " -O3 -funroll-loops " )
    ENDIF ( NNFW_CONFIG MATCHES debug )

    ### --- double check
    IF ( NNFW_CONFIG MATCHES double )
        ADD_DEFINITIONS( "-DNNFW_DOUBLE_PRECISION" )
    ENDIF ( NNFW_CONFIG MATCHES double )

    ### --- gsl check
    IF ( NNFW_CONFIG MATCHES gsl )
        ADD_DEFINITIONS( "-DNNFW_USE_GSL" )
    ENDIF ( NNFW_CONFIG MATCHES gsl )

    ### --- mkl check
    IF ( NNFW_CONFIG MATCHES mkl )
        IF ( NOT MKL_CACHED )
            EXEC_PROGRAM( "rpm -qa | grep mkl" OUTPUT_VARIABLE INTEL_MKL )
            EXEC_PROGRAM( "rpm -ql ${INTEL_MKL} | grep include$" OUTPUT_VARIABLE MKL_INC )
            SET( MKL_PATH ${MKL_INC} CACHE INTERNAL "MKL_PATH" )
            SET( MKL_CACHED TRUE CACHE INTERNAL "MKL_CACHED" )
        ENDIF ( NOT MKL_CACHED )
        ADD_DEFINITIONS( "-DNNFW_USE_MKL" )
    ENDIF ( NNFW_CONFIG MATCHES mkl )

    ### --- replace spurious ';' with blank space
    STRING( REPLACE ";" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} )
ENDIF (UNIX)

SET( NNFW_SRCS ./src/propertized.cpp ./src/cluster.cpp ./src/simplecluster.cpp ./src/biasedcluster.cpp ./src/fakecluster.cpp ./src/ddecluster.cpp ./src/clusterupdater.cpp ./src/libclusterupdaters.cpp ./src/matrixlinker.cpp ./src/sparsematrixlinker.cpp ./src/copylinker.cpp ./src/neuralnet.cpp ./src/utils.cpp ./src/nnfwfactory.cpp ./src/messages.cpp ./src/random.cpp ./src/types.cpp ./src/realvec.cpp ./src/realmat.cpp ./src/blockslearning.cpp ./src/learningnetwork.cpp )

SET( NNFW_HDRS ./include/nnfw.h ./include/primtypes.h ./include/types.h ./include/memutils.h ./include/observ.h ./include/clonable.h ./include/propertized.h ./include/vectordata.h ./include/matrixdata.h ./include/realvec.h ./include/realmat.h ./include/updatable.h ./include/cluster.h ./include/simplecluster.h ./include/biasedcluster.h ./include/fakecluster.h ./include/ddecluster.h ./include/clusterupdater.h ./include/derivableclusterupdater.h ./include/libclusterupdaters.h ./include/linker.h ./include/copylinker.h ./include/matrixlinker.h ./include/sparsematrixlinker.h ./include/neuralnet.h ./include/utils.h ./include/nnfwfactory.h ./include/messages.h ./include/random.h ./include/blockslearning.h ./include/learningnetwork.h )

INCLUDE_DIRECTORIES( ./include ${MKL_PATH} )
ADD_LIBRARY( nnfw STATIC ${NNFW_SRCS} )
SET(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)

INSTALL( TARGETS nnfw ARCHIVE DESTINATION lib )
INSTALL( FILES ${NNFW_HDRS} DESTINATION include/nnfw )

### On Linux Machines there are also the follow make target:
###  - make apidoc            <= generates documentation with doxygen
###  - make apidoc_clean      <= remove documentation generated
###  - make distclean         <= remove all file generated, also CMake generated files
IF (UNIX)
    ADD_CUSTOM_TARGET( apidoc doxygen )
    ADD_CUSTOM_TARGET( apidoc_clean rm -rf ./apidoc/html )
    ADD_CUSTOM_TARGET( distclean rm -rf CMakeFiles
        COMMAND rm -f CMakeCache.txt Makefile cmake_install.cmake install_manifest.txt
        COMMAND rm -f ./*~ ./src/*.cpp~ ./include/*.h~
        COMMAND rm -f lib*.a
        COMMAND rm -f ./lib/lib*.a
        COMMAND rm -f doxygen.log
        COMMAND rm -f ./apidoc/*~ )
ENDIF (UNIX)
