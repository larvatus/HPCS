

EXECUTE_PROCESS( COMMAND ln -s ${CMAKE_CURRENT_SOURCE_DIR}/data ${CMAKE_CURRENT_BINARY_DIR}/data  )
EXECUTE_PROCESS( COMMAND ln -s ${CMAKE_CURRENT_SOURCE_DIR}/dataSet.1.dat ${CMAKE_CURRENT_BINARY_DIR}/dataSet.1.dat  )
EXECUTE_PROCESS( COMMAND ln -s ${CMAKE_CURRENT_SOURCE_DIR}/dataSet.2.dat ${CMAKE_CURRENT_BINARY_DIR}/dataSet.2.dat  )

# First executable

ADD_EXECUTABLE(main_matrixDistances.exe main_matrixDistances )
TARGET_LINK_LIBRARIES(main_matrixDistances.exe ${MPI_LIBRARIES} mbd)
