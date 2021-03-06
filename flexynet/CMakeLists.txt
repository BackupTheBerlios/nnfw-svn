PROJECT( FLEXYNET )
SET( CMAKE_COLOR_MAKEFILE ON )
cmake_minimum_required(VERSION 2.4)
if(COMMAND cmake_policy)
	cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)


### configure CMake to link against Qt4
FIND_PACKAGE( Qt4 REQUIRED )
SET( QT_USE_QTXML TRUE )
SET( QT_USE_QTOPENGL TRUE )
INCLUDE(${QT_USE_FILE})

FILE( GLOB FLEXLIB_SRCS ./src/*.cpp )
FILE( GLOB FLEXLIB_HDRS ./include/*.h )
FILE( GLOB FLEXBIN_SRCS ./src/bin/*.cpp )
FILE( GLOB FLEXBIN_HDRS ./include/bin/*.h )

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

## extracting information from nnfw-config
EXEC_PROGRAM( nnfw-config ARGS --cpplibs OUTPUT_VARIABLE NNFW_LIBS )
EXEC_PROGRAM( nnfw-config ARGS --cflags OUTPUT_VARIABLE NNFW_CFLAGS )

####################### FLEYNET LIBRARY ################
### MOC WRAPPING
QT4_WRAP_CPP( FLEXLIB_SRCS ${FLEXLIB_HDRS} )
ADD_LIBRARY( flexynn SHARED ${FLEXLIB_SRCS} )
SET_TARGET_PROPERTIES( flexynn PROPERTIES COMPILE_FLAGS "${NNFW_CFLAGS}" )
SET_TARGET_PROPERTIES( flexynn PROPERTIES LINK_FLAGS "${NNFW_LIBS}" )
TARGET_LINK_LIBRARIES( flexynn ${QT_LIBRARIES} )

####################### FLEYNET APPLICATION ############
### RESOURCES
QT4_ADD_RESOURCES( FLEXBIN_SRCS "icons/icons.qrc" )
### MOC WRAPPING
QT4_WRAP_CPP( FLEXBIN_SRCS ${FLEXBIN_HDRS} )
### EXE SETTING
ADD_EXECUTABLE( flexynet ${FLEXBIN_SRCS} )
SET_TARGET_PROPERTIES( flexynet PROPERTIES COMPILE_FLAGS " ${NNFW_CFLAGS} -I${FLEXYNET_SOURCE_DIR}/include/bin" )
SET_TARGET_PROPERTIES( flexynet PROPERTIES LINK_FLAGS "${NNFW_LIBS}" )
TARGET_LINK_LIBRARIES( flexynet ${QT_LIBRARIES} )
TARGET_LINK_LIBRARIES( flexynet "flexynn" )
ADD_DEPENDENCIES( flexynet flexynn )

################# INSTALLATION #######
IF (WIN32)
	INSTALL( FILES "${FLEXYNET_SOURCE_DIR}/GPL.txt" DESTINATION . )
	INSTALL( FILES "${FLEXYNET_SOURCE_DIR}/CHANGELOG" DESTINATION . )
	INSTALL( FILES "${FLEXYNET_SOURCE_DIR}/COPYING" DESTINATION . )
	INSTALL( FILES "${FLEXYNET_SOURCE_DIR}/CREDITS" DESTINATION . )
	INSTALL( FILES "${FLEXYNET_SOURCE_DIR}/INSTALL" DESTINATION . )
	INSTALL( FILES "${FLEXYNET_SOURCE_DIR}/README" DESTINATION . )
	INSTALL( FILES "${FLEXYNET_SOURCE_DIR}/RELEASENOTES" DESTINATION . )
ELSE (WIN32)
	INSTALL( FILES "${FLEXYNET_SOURCE_DIR}/GPL.txt" DESTINATION share/flexynet )
	INSTALL( FILES "${FLEXYNET_SOURCE_DIR}/CHANGELOG" DESTINATION share/flexynet )
	INSTALL( FILES "${FLEXYNET_SOURCE_DIR}/COPYING" DESTINATION share/flexynet )
	INSTALL( FILES "${FLEXYNET_SOURCE_DIR}/CREDITS" DESTINATION share/flexynet )
	INSTALL( FILES "${FLEXYNET_SOURCE_DIR}/INSTALL" DESTINATION share/flexynet )
	INSTALL( FILES "${FLEXYNET_SOURCE_DIR}/README" DESTINATION share/flexynet )
	INSTALL( FILES "${FLEXYNET_SOURCE_DIR}/RELEASENOTES" DESTINATION share/flexynet )
ENDIF (WIN32)
INSTALL( FILES ${FLEXLIB_HDRS} DESTINATION include/flexynet )
INSTALL( TARGETS flexynn LIBRARY DESTINATION ${LIB_DIR} )
INSTALL( TARGETS flexynet RUNTIME DESTINATION bin )

######### Setting Compilers #########
IF( CMAKE_BUILD_TYPE MATCHES Debug OR CMAKE_BUILD_TYPE MATCHES RelWithDebInfo )
	ADD_DEFINITIONS( -DFLEX_DEBUG )
ENDIF( CMAKE_BUILD_TYPE MATCHES Debug OR CMAKE_BUILD_TYPE MATCHES RelWithDebInfo )
### Linux gcc compiler
IF (UNIX)
    MESSAGE( "-- Setting compiler for Linux" )
    SET( CMAKE_CXX_COMPILER "g++" )
    SET( CMAKE_CXX_FLAGS "-pipe -fPIC -Wall -W " )
    SET( CMAKE_CXX_FLAGS_DEBUG "-g -O0 " )
    SET( CMAKE_CXX_FLAGS_RELEASE "-O3 " )
    SET( CMAKE_CXX_FLAGS_RELWITHDEBINFO "-g -O2" )
    SET( CMAKE_CXX_FLAGS_MINSIZEREL "-O2 -Os" )
ENDIF (UNIX)

### Setting For Win32 machines
IF( WIN32 AND NOT UNIX )
	ADD_DEFINITIONS( -DFLEX_BUILDING_DLL )
ENDIF( WIN32 AND NOT UNIX )
