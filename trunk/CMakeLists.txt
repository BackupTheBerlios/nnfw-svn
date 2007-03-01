
PROJECT( NNFW )
SET( CMAKE_COLOR_MAKEFILE ON )

### Configure of Library
### a string variable that contains configuration keys separated by with spaces:
### - double     <= use double precision for Real number
### - gsl        <= link against GSL library
### - mkl        <= link against MKL library
### - shared     <= generates a shared library
### Default (empty) means: static, single precision, without GSL & MKL.
SET( NNFW_CONFIG "" CACHE STRING "NNFW Configuration" )

### configure CMake to link against Qt4
SET( QT_DONT_USE_QTGUI TRUE )
SET( QT_USE_QTXML TRUE )
INCLUDE( FindQt4 )
INCLUDE( UseQt4 )

FILE( GLOB NNFW_SRCS ./src/*.cpp )
FILE( GLOB NNFW_HDRS ./include/*.h )

INCLUDE_DIRECTORIES( ./include )
SET(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)
IF ( NNFW_CONFIG MATCHES shared )
    ADD_LIBRARY( nnfw SHARED ${NNFW_SRCS} )
    INSTALL( TARGETS nnfw LIBRARY DESTINATION lib )
ELSE ( NNFW_CONFIG MATCHES shared )
    ADD_LIBRARY( nnfw STATIC ${NNFW_SRCS} )
    INSTALL( TARGETS nnfw ARCHIVE DESTINATION lib )
ENDIF ( NNFW_CONFIG MATCHES shared )

INSTALL( FILES ${NNFW_HDRS} DESTINATION include/nnfw )
INSTALL( PROGRAMS ${NNFW_BINARY_DIR}/nnfw-config DESTINATION bin )

### Setting Compiler g++ for linux machines
IF (UNIX)
    MESSAGE( "-- Setting compiler for Linux" )
    SET( CMAKE_CXX_COMPILER "g++" )
    SET( CMAKE_CXX_FLAGS "-pipe -fno-strict-aliasing -Wall -W" )
    SET( CMAKE_CXX_FLAGS_DEBUG "-g -O0 -DNNFW_DEBUG" )
    SET( CMAKE_CXX_FLAGS_RELEASE "-O3 -funroll-loops" )
    SET( CMAKE_CXX_FLAGS_RELWITHDEBINFO "-g -O2" )
    SET( CMAKE_CXX_FLAGS_MINSIZEREL "-O2 -Os" )
    SET_TARGET_PROPERTIES( nnfw PROPERTIES COMPILE_FLAGS "" )
    ### --- double check
    IF ( NNFW_CONFIG MATCHES double )
        ADD_DEFINITIONS( "-DNNFW_DOUBLE_PRECISION" )
    ENDIF ( NNFW_CONFIG MATCHES double )
    ### --- gsl check
    IF ( NNFW_CONFIG MATCHES gsl )
        EXEC_PROGRAM( gsl-config ARGS --libs OUTPUT_VARIABLE GSL_LIB )
        EXEC_PROGRAM( gsl-config ARGS --cflags OUTPUT_VARIABLE GSL_FLAGS )
        GET_TARGET_PROPERTY( TMP nnfw COMPILE_FLAGS )
        SET_TARGET_PROPERTIES( nnfw PROPERTIES COMPILE_FLAGS "${TMP} ${GSL_FLAGS} -DNNFW_USE_GSL" )
    ENDIF ( NNFW_CONFIG MATCHES gsl )
    ### --- mkl check
    IF ( NNFW_CONFIG MATCHES mkl )
        IF ( NOT MKL_CACHED )
            EXEC_PROGRAM( "rpm -qa | grep mkl" OUTPUT_VARIABLE INTEL_MKL )
            EXEC_PROGRAM( "rpm -ql ${INTEL_MKL} | grep include$" OUTPUT_VARIABLE MKL_INC )
            SET( MKL_PATH ${MKL_INC} CACHE INTERNAL "MKL_PATH" )
            SET( MKL_CACHED TRUE CACHE INTERNAL "MKL_CACHED" )
        ENDIF ( NOT MKL_CACHED )
        GET_TARGET_PROPERTY( TMP nnfw COMPILE_FLAGS )
        SET_TARGET_PROPERTIES( nnfw PROPERTIES COMPILE_FLAGS "${TMP} -I${MKL_PATH} -DNNFW_USE_MKL" )
    ENDIF ( NNFW_CONFIG MATCHES mkl )
ENDIF (UNIX)

### create the nnfw-config script
IF (UNIX)
    ## prefix
    STRING( REPLACE "/" "\\/" PREF "${CMAKE_INSTALL_PREFIX}" )
    ## CFLAGS
    IF ( NNFW_CONFIG MATCHES double )
        SET( CFL " -DNNFW_DOUBLE_PRECISION " )
    ENDIF ( NNFW_CONFIG MATCHES double )
    ## Library
    IF ( NNFW_CONFIG MATCHES mkl )
        SET( LFLA "-Wl,-rpath,${MKL_PATH}/../lib/32 -L${MKL_PATH}/../lib/32 -lmkl_ia32 -lmkl -lvml -lguide" )
    ENDIF ( NNFW_CONFIG MATCHES mkl )
    SET( LFLA "-Wl,-rpath,${CMAKE_INSTALL_PREFIX}/lib ${LFLA} ${GSL_LIB} ${QT_QTCORE_LIBRARY} ${QT_QTXML_LIBRARY}" )
    STRING( REPLACE "/" "\\/" LFL "${LFLA}" )

    ADD_CUSTOM_COMMAND( TARGET nnfw POST_BUILD
        COMMAND sed 's/version=".*"/version="0.7.0"/' ${NNFW_SOURCE_DIR}/bin/nnfw-config_template | sed 's/prefix=".*"/prefix="${PREF}"/' | sed 's/^cflags=".*"/cflags="${CFL}"/' | sed 's/LIBS=".*"/LIBS="${LFL}"/' > nnfw-config 
    )
ENDIF (UNIX)

