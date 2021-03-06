PROJECT( NNFW )
SET( CMAKE_COLOR_MAKEFILE ON )
cmake_minimum_required( VERSION 2.4 )

###### VERSION INFORMATION
SET( VER_MAJ 1 )
SET( VER_MIN 1 )
SET( VER_PAT 5 )

### Configure of Library
### a string variable that contains configuration keys separated by with spaces:
### - double     <= use double precision for Real number
### - gsl        <= link against GSL library
### - mkl        <= link against MKL library
### - shared     <= generates a shared library
### Default (empty) means: static, single precision, without GSL & MKL.
SET( NNFW_CONFIG "" CACHE STRING "NNFW Configuration" )

### configure CMake to link against Qt4
FIND_PACKAGE( Qt4 REQUIRED )
SET( QT_DONT_USE_QTGUI TRUE )
SET( QT_USE_QTXML TRUE )
INCLUDE(${QT_USE_FILE})

FILE( GLOB NNFW_SRCS ./src/*.cpp )
FILE( GLOB NNFW_HDRS ./include/*.h )

FILE( GLOB C_NNFW_SRCS ./cinterface/*.cpp )
FILE( GLOB C_NNFW_HDRS ./cinterface/*.h )

INCLUDE_DIRECTORIES( ./include )

SET( USE_FRAMEWORKS FALSE )
IF ( NNFW_CONFIG MATCHES shared )
	ADD_LIBRARY( nnfw SHARED ${NNFW_SRCS} ${NNFW_HDRS} ${NNFW_BINARY_DIR}/NNFWConfig.cmake GPL.txt CHANGELOG COPYING CREDITS INSTALL README RELEASENOTES )
	ADD_LIBRARY( cnnfw SHARED ${C_NNFW_SRCS} ${NNFW_SRCS} ${C_NNFW_HDRS} GPL.txt CHANGELOG COPYING CREDITS INSTALL README RELEASENOTES )
	IF( CMAKE_CACHE_MAJOR_VERSION EQUAL 2 AND CMAKE_CACHE_MINOR_VERSION LESS 5 )
		INSTALL( TARGETS nnfw LIBRARY DESTINATION lib )
		INSTALL( TARGETS cnnfw LIBRARY DESTINATION lib )
	ELSE( CMAKE_CACHE_MAJOR_VERSION EQUAL 2 AND CMAKE_CACHE_MINOR_VERSION LESS 5 )
		IF( APPLE )
			INSTALL( TARGETS nnfw LIBRARY DESTINATION ./ FRAMEWORK DESTINATION ./ )
			SET_TARGET_PROPERTIES( nnfw PROPERTIES
				FRAMEWORK TRUE
				FRAMEWORK_VERSION "${VER_MAJ}.${VER_MIN}"
				PRIVATE_HEADER ""
				PUBLIC_HEADER "${NNFW_HDRS}"
				RESOURCE "NNFWConfig.cmake;GPL.txt;CHANGELOG;COPYING;CREDITS;INSTALL;README;RELEASENOTES"
			)
			INSTALL( TARGETS cnnfw LIBRARY DESTINATION ./ FRAMEWORK DESTINATION ./ )
			SET_TARGET_PROPERTIES( cnnfw PROPERTIES
				FRAMEWORK TRUE
				FRAMEWORK_VERSION "${VER_MAJ}.${VER_MIN}"
				PRIVATE_HEADER ""
				PUBLIC_HEADER "${C_NNFW_HDRS}"
				RESOURCE "GPL.txt;CHANGELOG;COPYING;CREDITS;INSTALL;README;RELEASENOTES"
			)
			SET( USE_FRAMEWORKS TRUE )
		ELSE( APPLE )
			INSTALL( TARGETS nnfw ARCHIVE DESTINATION lib LIBRARY DESTINATION lib RUNTIME DESTINATION bin )
			INSTALL( TARGETS cnnfw ARCHIVE DESTINATION lib LIBRARY DESTINATION lib RUNTIME DESTINATION bin )
		ENDIF( APPLE )
	ENDIF( CMAKE_CACHE_MAJOR_VERSION EQUAL 2 AND CMAKE_CACHE_MINOR_VERSION LESS 5 )
ELSE ( NNFW_CONFIG MATCHES shared )
	ADD_LIBRARY( nnfw STATIC ${NNFW_SRCS} )
	INSTALL( TARGETS nnfw ARCHIVE DESTINATION lib )
	ADD_LIBRARY( cnnfw STATIC ${C_NNFW_SRCS} ${NNFW_SRCS} )
	INSTALL( TARGETS cnnfw ARCHIVE DESTINATION lib )
ENDIF ( NNFW_CONFIG MATCHES shared )

SET_TARGET_PROPERTIES( nnfw PROPERTIES VERSION ${VER_MAJ}.${VER_MIN}.${VER_PAT} SOVERSION ${VER_MAJ}.${VER_MIN} )
TARGET_LINK_LIBRARIES( nnfw ${QT_LIBRARIES} )
TARGET_LINK_LIBRARIES( cnnfw ${QT_LIBRARIES} )

IF( WIN32 )
	INSTALL( FILES "${NNFW_SOURCE_DIR}/GPL.txt" DESTINATION . )
	INSTALL( FILES "${NNFW_SOURCE_DIR}/CHANGELOG" DESTINATION . )
	INSTALL( FILES "${NNFW_SOURCE_DIR}/COPYING" DESTINATION . )
	INSTALL( FILES "${NNFW_SOURCE_DIR}/CREDITS" DESTINATION . )
	INSTALL( FILES "${NNFW_SOURCE_DIR}/INSTALL" DESTINATION . )
	INSTALL( FILES "${NNFW_SOURCE_DIR}/README" DESTINATION . )
	INSTALL( FILES "${NNFW_SOURCE_DIR}/RELEASENOTES" DESTINATION . )
	INSTALL( FILES ${NNFW_HDRS} DESTINATION include/nnfw )
	INSTALL( FILES ${C_NNFW_HDRS} DESTINATION include/nnfw )
ELSE( WIN32 )
IF(NOT USE_FRAMEWORKS )
	INSTALL( FILES "${NNFW_SOURCE_DIR}/GPL.txt" DESTINATION share/nnfw )
	INSTALL( FILES "${NNFW_SOURCE_DIR}/CHANGELOG" DESTINATION share/nnfw )
	INSTALL( FILES "${NNFW_SOURCE_DIR}/COPYING" DESTINATION share/nnfw )
	INSTALL( FILES "${NNFW_SOURCE_DIR}/CREDITS" DESTINATION share/nnfw )
	INSTALL( FILES "${NNFW_SOURCE_DIR}/INSTALL" DESTINATION share/nnfw )
	INSTALL( FILES "${NNFW_SOURCE_DIR}/README" DESTINATION share/nnfw )
	INSTALL( FILES "${NNFW_SOURCE_DIR}/RELEASENOTES" DESTINATION share/nnfw )
	INSTALL( FILES ${NNFW_HDRS} DESTINATION include/nnfw )
	INSTALL( FILES ${C_NNFW_HDRS} DESTINATION include/nnfw )
ENDIF(NOT USE_FRAMEWORKS )
ENDIF( WIN32 )

### Setting Compiler g++ for linux machines
IF (UNIX)
    MESSAGE( "-- Setting compiler for Linux" )
    SET( CMAKE_CXX_COMPILER "g++" )
    SET( CMAKE_CXX_FLAGS "-pipe -fPIC -Wall -W " )
    SET( CMAKE_CXX_FLAGS_DEBUG "-g -O0 " )
    SET( CMAKE_CXX_FLAGS_RELEASE "-O3  " )
    SET( CMAKE_CXX_FLAGS_RELWITHDEBINFO "-g -O2 " )
    SET( CMAKE_CXX_FLAGS_MINSIZEREL "-O2 -Os" )
    SET_TARGET_PROPERTIES( nnfw PROPERTIES COMPILE_FLAGS "" )
    SET_TARGET_PROPERTIES( nnfw PROPERTIES LINK_FLAGS "" )
    SET_TARGET_PROPERTIES( cnnfw PROPERTIES COMPILE_FLAGS " -DNNFW_DONT_EXPORT " )
    SET_TARGET_PROPERTIES( cnnfw PROPERTIES LINK_FLAGS " -static-libgcc " )
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
        GET_TARGET_PROPERTY( TMP cnnfw COMPILE_FLAGS )
        SET_TARGET_PROPERTIES( cnnfw PROPERTIES COMPILE_FLAGS "${TMP} ${GSL_FLAGS} -DNNFW_USE_GSL" )
        GET_TARGET_PROPERTY( TMP nnfw LINK_FLAGS )
        SET_TARGET_PROPERTIES( nnfw PROPERTIES LINK_FLAGS "${TMP} ${GSL_LIB} -DNNFW_USE_GSL" )
        GET_TARGET_PROPERTY( TMP cnnfw LINK_FLAGS )
        SET_TARGET_PROPERTIES( cnnfw PROPERTIES LINK_FLAGS "${TMP} ${GSL_LIB} -DNNFW_USE_GSL" )
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
        GET_TARGET_PROPERTY( TMP cnnfw COMPILE_FLAGS )
        SET_TARGET_PROPERTIES( cnnfw PROPERTIES COMPILE_FLAGS "${TMP} -I${MKL_PATH} -DNNFW_USE_MKL" )
    ENDIF ( NNFW_CONFIG MATCHES mkl )
ENDIF (UNIX)

### Generate the NNFWConfig.cmake
IF( UNIX )
	IF( APPLE )
		LIST( APPEND INCLUDES "${CMAKE_INSTALL_PREFIX}/nnfw.framework/Headers" )
	ELSE( APPLE )
		LIST( APPEND INCLUDES "${CMAKE_INSTALL_PREFIX}/include" )
	ENDIF( APPLE )
	IF( CMAKE_BUILD_TYPE MATCHES "Debug" )
		#LIST( APPEND DEFS "-DNNFW_DEBUG" )
	ENDIF( CMAKE_BUILD_TYPE MATCHES "Debug" )
	IF( APPLE )
		LIST( APPEND DEFS "-DNNFW_MAC" )
	ELSE( APPLE )
		LIST( APPEND DEFS "-DNNFW_LINUX" )
	ENDIF( APPLE )
	IF( USE_FRAMEWORKS )
		LIST( APPEND LIBRARIES "-framework nnfw" )
		LIST( APPEND LFLAGS "-F${CMAKE_INSTALL_PREFIX}" )
	ELSE( USE_FRAMEWORKS )
		LIST( APPEND LIBRARIES "nnfw" )
		LIST( APPEND LFLAGS "-L${CMAKE_INSTALL_PREFIX}/lib -Wl,-rpath,${CMAKE_INSTALL_PREFIX}/lib" )
	ENDIF( USE_FRAMEWORKS )
	IF ( NOT APPLE ) 
		INSTALL( FILES "${NNFW_BINARY_DIR}/NNFWConfig.cmake" DESTINATION share/nnfw )
	ENDIF (NOT APPLE)
ENDIF( UNIX )
IF ( WIN32 )
	LIST( APPEND INCLUDES "${CMAKE_INSTALL_PREFIX}/include" )
	LIST( APPEND DEFS "-DNNFW_WIN" )
	IF ( NNFW_CONFIG MATCHES shared )
		## riga vuota
	ELSE( NNFW_CONFIG MATCHES shared )
		LIST( APPEND DEFS "-DNNFW_STATIC" )
	ENDIF( NNFW_CONFIG MATCHES shared )

	IF( CMAKE_BUILD_TYPE MATCHES "Debug" )
		LIST( APPEND DEFS "-DNNFW_DEBUG " )
	ENDIF( CMAKE_BUILD_TYPE MATCHES "Debug" )
	LIST( APPEND LIBRARIES "optimized;nnfw;libgsl;libgslcblas" "debug;nnfwd;libgsld;libgslcblasd" )
	LIST( APPEND LINKDIRS "${CMAKE_INSTALL_PREFIX}/lib" )
	INSTALL( FILES "${NNFW_BINARY_DIR}/NNFWConfig.cmake" DESTINATION . )
ENDIF( WIN32)
### --- double check
IF ( NNFW_CONFIG MATCHES double )
	LIST( APPEND DEFS "-DNNFW_DOUBLE_PRECISION" )
ENDIF ( NNFW_CONFIG MATCHES double )
### --- gsl check
IF ( NNFW_CONFIG MATCHES gsl )
	LIST( APPEND DEFS "-DNNFW_USE_GSL" )
ENDIF ( NNFW_CONFIG MATCHES gsl )
CONFIGURE_FILE( ${NNFW_SOURCE_DIR}/NNFWConfig.cmake.in ${NNFW_BINARY_DIR}/NNFWConfig.cmake @ONLY IMMEDIATE )


### create the nnfw-config script
IF (UNIX)
    ## CFLAGS
    IF ( NNFW_CONFIG MATCHES double )
        SET( CFLA " -DNNFW_DOUBLE_PRECISION " )
    ENDIF ( NNFW_CONFIG MATCHES double )
    IF ( NNFW_CONFIG MATCHES mkl )
		SET( CFLA "${CFLA} -I${MKL_PATH}" )
	ENDIF ( NNFW_CONFIG MATCHES mkl )
	## include directory
	IF( APPLE )
		SET( INCLUDES "${CMAKE_INSTALL_PREFIX}/nnfw.framework/Headers" )
	ELSE( APPLE )
		SET( INCLUDES "${CMAKE_INSTALL_PREFIX}/include" )
	ENDIF( APPLE )
    ## Library
	IF ( NNFW_CONFIG MATCHES shared )
		IF ( NNFW_CONFIG MATCHES mkl )
			SET( LFLA "-Wl,-rpath,${MKL_PATH}/../lib/32 -L${MKL_PATH}/../lib/32 -lvml " )
		ENDIF ( NNFW_CONFIG MATCHES mkl )
		IF( USE_FRAMEWORKS )
			SET( LFLA " -F${CMAKE_INSTALL_PREFIX} ${LFLA}" )
			SET( CLIB " -framework cnnfw " )
			SET( CPPLIB " -framework nnfw " )
		ELSE( USE_FRAMEWORKS )
			SET( LFLA "-L${CMAKE_INSTALL_PREFIX}/lib -Wl,-rpath,${CMAKE_INSTALL_PREFIX}/${LIB_DIR} ${LFLA}" )
			SET( CLIB " -lcnnfw " )
			SET( CPPLIB " -lnnfw " )
		ENDIF( USE_FRAMEWORKS )
	ELSE ( NNFW_CONFIG MATCHES shared )
        EXEC_PROGRAM( gsl-config ARGS --libs-without-cblas OUTPUT_VARIABLE GSL_ELIB )
		IF ( NNFW_CONFIG MATCHES mkl )
			SET( LFLA "-Wl,-rpath,${MKL_PATH}/../lib/32 -L${MKL_PATH}/../lib/32 -lmkl -lvml " )
		ENDIF ( NNFW_CONFIG MATCHES mkl )
		IF( APPLE )
			SET( LFLA "-L${CMAKE_INSTALL_PREFIX}/lib ${LFLA} ${GSL_ELIB} -F${QT_LIBRARY_DIR} -framework QtXml" )
		ELSE( APPLE )
			SET( LFLA "-L${CMAKE_INSTALL_PREFIX}/lib ${LFLA} ${GSL_ELIB} -L${QT_LIBRARY_DIR} -lQtXml" )
		ENDIF( APPLE )
		SET( CLIB " -lcnnfw " )
		SET( CPPLIB " -lnnfw " )
	ENDIF ( NNFW_CONFIG MATCHES shared )

	CONFIGURE_FILE( ${NNFW_SOURCE_DIR}/bin/nnfw-config_template ${NNFW_BINARY_DIR}/nnfw-config @ONLY IMMEDIATE )

IF( USE_FRAMEWORKS )
	ADD_CUSTOM_COMMAND( TARGET nnfw POST_BUILD
		COMMAND chmod +x nnfw-config
		COMMAND cp nnfw-config nnfw.framework
		COMMAND install_name_tool -id ${CMAKE_INSTALL_PREFIX}/nnfw.framework/nnfw nnfw.framework/nnfw
		COMMAND install_name_tool -id ${CMAKE_INSTALL_PREFIX}/cnnfw.framework/nnfw cnnfw.framework/cnnfw
	)
	ADD_CUSTOM_COMMAND( TARGET cnnfw POST_BUILD
		COMMAND chmod +x nnfw-config
		COMMAND cp nnfw-config cnnfw.framework
		COMMAND install_name_tool -id ${CMAKE_INSTALL_PREFIX}/cnnfw.framework/nnfw cnnfw.framework/cnnfw
	)
ELSE( USE_FRAMEWORKS )
	INSTALL( PROGRAMS ${NNFW_BINARY_DIR}/nnfw-config DESTINATION bin )
ENDIF( USE_FRAMEWORKS )
ENDIF (UNIX)

### Setting For Win32 machines
IF( WIN32 AND NOT UNIX )
	set_target_properties(nnfw PROPERTIES DEBUG_POSTFIX "d")
	set_target_properties(cnnfw PROPERTIES DEBUG_POSTFIX "d")
	IF ( NNFW_CONFIG MATCHES shared )
		ADD_DEFINITIONS( -DNNFW_BUILDING_DLL )
	ELSE( NNFW_CONFIG MATCHES shared )
		ADD_DEFINITIONS( -DNNFW_STATIC )
	ENDIF( NNFW_CONFIG MATCHES shared )
	IF( CMAKE_BUILD_TYPE MATCHES Debug OR CMAKE_BUILD_TYPE MATCHES RelWithDebInfo )
		ADD_DEFINITIONS( -DNNFW_DEBUG )
	ENDIF( CMAKE_BUILD_TYPE MATCHES Debug OR CMAKE_BUILD_TYPE MATCHES RelWithDebInfo )
    ### --- double check
    IF ( NNFW_CONFIG MATCHES double )
        ADD_DEFINITIONS( "-DNNFW_DOUBLE_PRECISION" )
    ENDIF ( NNFW_CONFIG MATCHES double )
    SET_TARGET_PROPERTIES( cnnfw PROPERTIES COMPILE_FLAGS " /DNNFW_DONT_EXPORT " )
    ### --- gsl check
    IF ( NNFW_CONFIG MATCHES gsl )
		INCLUDE_DIRECTORIES( "$ENV{GSL_DIR}/include" )
		ADD_DEFINITIONS( -DNNFW_USE_GSL )
		TARGET_LINK_LIBRARIES( nnfw optimized "libgsl" debug "libgsld" )
		TARGET_LINK_LIBRARIES( nnfw optimized "libgslcblas" debug "libgslcblasd" )
		SET_TARGET_PROPERTIES( nnfw PROPERTIES LINK_FLAGS "/LIBPATH:$ENV{GSL_DIR}\\lib" )
		TARGET_LINK_LIBRARIES( cnnfw optimized "libgsl" debug "libgsld" )
		TARGET_LINK_LIBRARIES( cnnfw optimized "libgslcblas" debug "libgslcblasd" )
		SET_TARGET_PROPERTIES( cnnfw PROPERTIES LINK_FLAGS "/LIBPATH:$ENV{GSL_DIR}\\lib" )
    ENDIF ( NNFW_CONFIG MATCHES gsl )

	TARGET_LINK_LIBRARIES( nnfw "QtXml4" )
	TARGET_LINK_LIBRARIES( nnfw "QtCore4" )
	TARGET_LINK_LIBRARIES( cnnfw "QtXml4" )
	TARGET_LINK_LIBRARIES( cnnfw "QtCore4" )

ENDIF( WIN32 AND NOT UNIX )

