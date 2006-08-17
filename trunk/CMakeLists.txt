
PROJECT( NNFW )

SET( NNFW_BINARY_DIR ./lib )
SET( NNFW_SRCS ./src/cluster.cpp ./src/simplecluster.cpp ./src/biasedcluster.cpp ./src/fakecluster.cpp ./src/ddecluster.cpp ./src/clusterupdater.cpp ./src/libclusterupdaters.cpp ./src/matrixlinker.cpp ./src/sparsematrixlinker.cpp ./src/copylinker.cpp ./src/neuralnet.cpp ./src/utils.cpp ./src/nnfwfactory.cpp ./src/messages.cpp ./src/random.cpp ./src/types.cpp ./src/realvec.cpp ./src/realmat.cpp ./src/blockslearning.cpp ./src/learningnetwork.cpp )

SET( NNFW_HDRS ./include/nnfw.h ./include/primtypes.h ./include/types.h ./include/memutils.h ./include/observ.h ./include/clonable.h ./include/vectordata.h ./include/matrixdata.h ./include/realvec.h ./include/realmat.h ./include/updatable.h ./include/cluster.h ./include/simplecluster.h ./include/biasedcluster.h ./include/fakecluster.h ./include/ddecluster.h ./include/clusterupdater.h ./include/derivableclusterupdater.h ./include/libclusterupdaters.h ./include/linker.h ./include/copylinker.h ./include/matrixlinker.h ./include/sparsematrixlinker.h ./include/neuralnet.h ./include/utils.h ./include/nnfwfactory.h ./include/messages.h ./include/random.h ./include/blockslearning.h ./include/learningnetwork.h )

INCLUDE_DIRECTORIES( ./include )
ADD_LIBRARY( nnfw STATIC ${NNFW_SRCS} )

INSTALL( TARGETS nnfw ARCHIVE DESTINATION lib )
INSTALL( FILES ${NNFW_HDRS} DESTINATION include/nnfw )

ADD_CUSTOM_TARGET( apidoc doxygen )
ADD_CUSTOM_TARGET( apidoc_clean rm -rf ./apidoc/html )

ADD_CUSTOM_TARGET( distclean rm -rf CMakeFiles
        COMMAND rm -f CMakeCache.txt Makefile cmake_install.cmake install_manifest.txt
        COMMAND rm -f ./*~ ./src/*.cpp~ ./include/*.h~
        COMMAND rm -f lib*.a
        COMMAND rm -f ./lib/lib*.a
        COMMAND rm -f doxygen.log
        COMMAND rm -f ./apidoc/*~ )

