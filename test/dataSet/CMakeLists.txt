EXECUTE_PROCESS( COMMAND ln -s ${CMAKE_CURRENT_SOURCE_DIR}/dataSet.dat ${CMAKE_CURRENT_BINARY_DIR}/dataSet.dat  )


# First executable

ADD_EXECUTABLE(main_dataSet.exe main_dataSet )
TARGET_LINK_LIBRARIES(main_dataSet.exe ${MPI_LIBRARIES} mbd)
# ENABLE_TESTING()
# ADD_TEST(mytest  ${MPIEXEC} ${MPIEXEC_NUMPROC_FLAG} 4 ${CMAKE_CURRENT_BINARY_DIR}/mainDataSet.exe )

# Second executable

ADD_EXECUTABLE(main_dataSetLevelled.exe main_dataSetLevelled )
TARGET_LINK_LIBRARIES(main_dataSetLevelled.exe ${MPI_LIBRARIES} mbd)
# ENABLE_TESTING()
# ADD_TEST(mytest  ${MPIEXEC} ${MPIEXEC_NUMPROC_FLAG} 4 ${CMAKE_CURRENT_BINARY_DIR}/mainDataSet.exe )
