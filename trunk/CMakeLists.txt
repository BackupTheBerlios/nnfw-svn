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
FIND_PACKAGE( Qt4 REQUIRED )
SET( QT_DONTUSE_QTGUI TRUE )
SET( QT_USE_QTXML TRUE )
INCLUDE(${QT_USE_FILE})

FILE( GLOB NNFW_SRCS ./src/*.cpp )
FILE( GLOB NNFW_HDRS ./include/*.h )

INCLUDE_DIRECTORIES( ./include )
### Directory where Library will be installed
SET( LIB_DIR "lib" )
IF (UNIX)
	### check the type of machine
	EXEC_PROGRAM( uname ARGS -m OUTPUT_VARIABLE ARCHTYPE )
	IF ( ARCHTYPE MATCHES "x86_64" )
		SET( LIB_DIR "lib64" )
	ENDIF ( ARCHTYPE MATCHES "x86_64" )
ENDIF (UNIX)

IF ( NNFW_CONFIG MATCHES shared )
    ADD_LIBRARY( nnfw SHARED ${NNFW_SRCS} )
    INSTALL( TARGETS nnfw LIBRARY DESTINATION ${LIB_DIR} )
IF(WIN32)
	INSTALL( FILES ${NNFW_BINARY_DIR}/release/nnfw.dll DESTINATION bin )
	INSTALL( FILES ${NNFW_BINARY_DIR}/release/nnfw.lib DESTINATION bin )
	INSTALL( FILES ${NNFW_BINARY_DIR}/release/nnfw.exp DESTINATION bin )
	INSTALL( FILES C:/Qt/4.2.3-vs/bin/QtXml4.dll DESTINATION bin )
	INSTALL( FILES C:/Qt/4.2.3-vs/bin/QtCore4.dll DESTINATION bin )
	## --- Debug versions
	INSTALL( FILES ${NNFW_BINARY_DIR}/debug/nnfw.dll DESTINATION bin RENAME nnfwd.dll )
	INSTALL( FILES ${NNFW_BINARY_DIR}/debug/nnfw.lib DESTINATION bin RENAME nnfwd.lib )
	INSTALL( FILES ${NNFW_BINARY_DIR}/debug/nnfw.exp DESTINATION bin RENAME nnfwd.exp )
ENDIF(WIN32)
ELSE ( NNFW_CONFIG MATCHES shared )
    ADD_LIBRARY( nnfw STATIC ${NNFW_SRCS} )
    INSTALL( TARGETS nnfw ARCHIVE DESTINATION ${LIB_DIR} )
ENDIF ( NNFW_CONFIG MATCHES shared )

IF (WIN32)
	INSTALL( FILES "${NNFW_SOURCE_DIR}/GPL.txt" DESTINATION . )
	INSTALL( FILES "${NNFW_SOURCE_DIR}/CHANGELOG" DESTINATION . )
	INSTALL( FILES "${NNFW_SOURCE_DIR}/COPYING" DESTINATION . )
	INSTALL( FILES "${NNFW_SOURCE_DIR}/CREDITS" DESTINATION . )
	INSTALL( FILES "${NNFW_SOURCE_DIR}/INSTALL" DESTINATION . )
	INSTALL( FILES "${NNFW_SOURCE_DIR}/README" DESTINATION . )
	INSTALL( FILES "${NNFW_SOURCE_DIR}/RELEASENOTES" DESTINATION . )
ELSE (WIN32)
	INSTALL( FILES "${NNFW_SOURCE_DIR}/GPL.txt" DESTINATION share/nnfw )
	INSTALL( FILES "${NNFW_SOURCE_DIR}/CHANGELOG" DESTINATION share/nnfw )
	INSTALL( FILES "${NNFW_SOURCE_DIR}/COPYING" DESTINATION share/nnfw )
	INSTALL( FILES "${NNFW_SOURCE_DIR}/CREDITS" DESTINATION share/nnfw )
	INSTALL( FILES "${NNFW_SOURCE_DIR}/INSTALL" DESTINATION share/nnfw )
	INSTALL( FILES "${NNFW_SOURCE_DIR}/README" DESTINATION share/nnfw )
	INSTALL( FILES "${NNFW_SOURCE_DIR}/RELEASENOTES" DESTINATION share/nnfw )
ENDIF (WIN32)
INSTALL( FILES ${NNFW_HDRS} DESTINATION include/nnfw )

# ---------------------------------------- #
# ############## PACKAGE ################# #
# ---------------------------------------- #
SET(CPACK_PACKAGE_FILE_NAME "nnfw-0.7.2")
SET(CPACK_PACKAGE_VERSION_MAJOR "0")
SET(CPACK_PACKAGE_VERSION_MINOR "7")
SET(CPACK_PACKAGE_VERSION_PATCH "2")
SET(CPACK_PACKAGE_VENDOR "Gianluca Massera")

SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Neural Network Framework, NNFW Project")
#SET(CPACK_PACKAGE_DESCRIPTION_FILE "")
SET(CPACK_RESOURCE_FILE_LICENSE "${NNFW_SOURCE_DIR}/GPL.txt")
#SET(CPACK_RESOURCE_FILE_README "")
#SET(CPACK_RESOURCE_FILE_WELCOME "")
#   CPACK_INSTALL_COMMANDS              - Extra commands to install components

IF(WIN32 AND NOT UNIX)
	# There is a bug in NSI that does not handle full unix paths properly. Make
	# sure there is at least one set of four (4) backlasshes.
	#SET(CPACK_PACKAGE_ICON "${CMake_SOURCE_DIR}/Utilities/Release\\\\InstallIcon.bmp")
	#SET(CPACK_NSIS_INSTALLED_ICON_NAME "bin\\\\MyExecutable.exe")
	SET(CPACK_NSIS_DISPLAY_NAME "${CPACK_PACKAGE_INSTALL_DIRECTORY} Neural Network Framework")
	SET(CPACK_NSIS_HELP_LINK "http:\\\\\\\\www.nnfw.org")
	SET(CPACK_NSIS_URL_INFO_ABOUT "http:\\\\\\\\www.nnfw.org")
	SET(CPACK_NSIS_CONTACT "mave@berlios.de")
	SET(CPACK_GENERATOR "NSIS")
ENDIF(WIN32 AND NOT UNIX)
IF (UNIX)
	SET(CPACK_GENERATOR "STGZ")
ENDIF (UNIX)

INCLUDE(CPack)


### Setting Compiler g++ for linux machines
IF (UNIX)
    MESSAGE( "-- Setting compiler for Linux" )
    SET( CMAKE_CXX_COMPILER "g++" )
    SET( CMAKE_CXX_FLAGS "-pipe -fPIC -Wall -W " )
    SET( CMAKE_CXX_FLAGS_DEBUG "-g -O0 -DNNFW_DEBUG" )
    SET( CMAKE_CXX_FLAGS_RELEASE "-O3 " )
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
	INSTALL( PROGRAMS ${NNFW_BINARY_DIR}/nnfw-config DESTINATION bin )
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
    SET( LFLA "-Wl,-rpath,${CMAKE_INSTALL_PREFIX}/${LIB_DIR} ${LFLA} ${GSL_LIB} ${QT_QTCORE_LIBRARY} ${QT_QTXML_LIBRARY}" )
	STRING( REPLACE "optimized" "" LFLA "${LFLA}" )
	STRING( REPLACE "debug" "" LFLA "${LFLA}" )
    STRING( REPLACE "/" "\\/" LFL "${LFLA}" )

    ADD_CUSTOM_COMMAND( TARGET nnfw POST_BUILD
        COMMAND sed 's/version=".*"/version="0.7.2"/' ${NNFW_SOURCE_DIR}/bin/nnfw-config_template | sed 's/prefix=".*"/prefix="${PREF}"/' | sed 's/^cflags=".*"/cflags="${CFL}"/' | sed 's/LIBS=".*"/LIBS="${LFL}"/' | sed 's/libdir=".*"/libdir="${PREF}\\/${LIB_DIR}"/' > nnfw-config 
    )
ENDIF (UNIX)

### Setting For Win32 machines
IF( WIN32 AND NOT UNIX )
	ADD_DEFINITIONS( -DNNFW_BUILDING_DLL )
IF( CMAKE_BUILD_TYPE MATCHES Debug OR CMAKE_BUILD_TYPE MATCHES RelWithDebInfo )
	ADD_DEFINITIONS( -DNNFW_DEBUG )
ENDIF( CMAKE_BUILD_TYPE MATCHES Debug OR CMAKE_BUILD_TYPE MATCHES RelWithDebInfo )
	TARGET_LINK_LIBRARIES( nnfw "QtXml4" )
	TARGET_LINK_LIBRARIES( nnfw "QtCore4" )
ENDIF( WIN32 AND NOT UNIX )

### Setting Compiler minGW for Windows machines
IF (MINGW)
    SET_TARGET_PROPERTIES( nnfw PROPERTIES LINK_FLAGS "-LC:\\Qt\\4.2.3\\lib -Wl,--output-def,nnfwdll.def -Wl,--export-all-symbols -Wl,--add-stdcall-alias" )
    
	## Create VisualStudio library
#    ADD_CUSTOM_COMMAND( TARGET nnfw POST_BUILD
#        COMMAND lib /machine:X86 /def:nnfwdll.def
#    )

ENDIF (MINGW)
