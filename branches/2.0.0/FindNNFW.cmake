
IF (NOT NNFW_FOUND)

EXEC_PROGRAM( nnfw-config ARGS --prefix OUTPUT_VARIABLE PREF )
IF ( PREF MATCHES NOT_FOUND )
	MESSAGE( " Error: nnfw-config scritp not found: try default " )
	INCLUDE( "/usr/local/include/nnfw/NNFWConfig.cmake" )
ELSE ( PREF MATCHES NOT_FOUND )
	INCLUDE( "${PREF}/include/nnfw/NNFWConfig.cmake" )
ENDIF ( PREF MATCHES NOT_FOUND )

ENDIF (NOT NNFW_FOUND)
