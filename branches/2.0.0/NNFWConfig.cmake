### configure CMake to link against Qt4
FIND_PACKAGE( Qt4 REQUIRED )
SET( QT_USE_QTXML TRUE )
INCLUDE(${QT_USE_FILE})

IF (NOT NNFW_FOUND)

MESSAGE(STATUS "Using NNFW version @VER_MAJ@.@VER_MIN@.@VER_PAT@")
MESSAGE(STATUS "Apply the Macro TARGET_USE_NNFW(targetname) for use NNFW")

###################################
### MACRO for C++ Project       ##
##################################
MACRO( TARGET_USE_NNFW targetname )

GET_TARGET_PROPERTY( OLD1 ${targetname} COMPILE_FLAGS )
IF ( OLD1 MATCHES NOTFOUND )
	SET( OLD1 "" )
ENDIF( OLD1 MATCHES NOTFOUND )
SET_TARGET_PROPERTIES( ${targetname} PROPERTIES COMPILE_FLAGS "${OLD1} @INCLUDES_CPP@" )
TARGET_LINK_LIBRARIES( ${targetname} ${QT_LIBRARIES} )
TARGET_LINK_LIBRARIES( ${targetname} "@LIBRARIES_CPP@" )

ENDMACRO(TARGET_USE_NNFW)


###################################
### MACRO for C Project         ##
##################################
MACRO( TARGET_USE_CNNFW targetname )

GET_TARGET_PROPERTY( OLD1 ${targetname} COMPILE_FLAGS )
IF ( OLD1 MATCHES NOTFOUND )
	SET( OLD1 "" )
ENDIF( OLD1 MATCHES NOTFOUND )
SET_TARGET_PROPERTIES( ${targetname} PROPERTIES COMPILE_FLAGS "${OLD1} @INCLUDES_C@" )
TARGET_LINK_LIBRARIES( ${targetname} ${QT_LIBRARIES} )
TARGET_LINK_LIBRARIES( ${targetname} "@LIBRARIES_C@" )

ENDMACRO(TARGET_USE_CNNFW)

SET (NNFW_FOUND TRUE)
ENDIF (NOT NNFW_FOUND)
