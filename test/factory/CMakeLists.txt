# First executable

EXECUTE_PROCESS( COMMAND ln -s ${CMAKE_CURRENT_SOURCE_DIR}/data ${CMAKE_CURRENT_BINARY_DIR}/data  )

ADD_EXECUTABLE( main_bandDepth.exe main_bandDepth )
TARGET_LINK_LIBRARIES(main_bandDepth.exe ${MPI_LIBRARIES} HPCS)
# ENABLE_TESTING()
# ADD_TEST(mytest  ${MPIEXEC} ${MPIEXEC_NUMPROC_FLAG} 4 ${CMAKE_CURRENT_BINARY_DIR}/mainDataSet.exe )


# Second executable
# 
 ADD_EXECUTABLE( main_bandDepthRef.exe main_bandDepthRef )
 TARGET_LINK_LIBRARIES( main_bandDepthRef.exe ${MPI_LIBRARIES} HPCS)
 # ENABLE_TESTING()
 # ADD_TEST(mytest  ${MPIEXEC} ${MPIEXEC_NUMPROC_FLAG} 4 ${CMAKE_CURRENT_BINARY_DIR}/mainDataSet.exe )
