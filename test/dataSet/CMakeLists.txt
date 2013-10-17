

EXECUTE_PROCESS( COMMAND ln -s ${CMAKE_CURRENT_SOURCE_DIR}/data ${CMAKE_CURRENT_BINARY_DIR}/data  )
EXECUTE_PROCESS( COMMAND ln -s ${CMAKE_CURRENT_SOURCE_DIR}/dataSet.dat ${CMAKE_CURRENT_BINARY_DIR}/dataSet.dat  )
EXECUTE_PROCESS( COMMAND ln -s ${CMAKE_CURRENT_SOURCE_DIR}/dataSet.eigen.dat ${CMAKE_CURRENT_BINARY_DIR}/dataSet.eigen.dat  )
EXECUTE_PROCESS( COMMAND ln -s ${CMAKE_CURRENT_SOURCE_DIR}/levels.dat ${CMAKE_CURRENT_BINARY_DIR}/levels.dat  )

# First executable

ADD_EXECUTABLE(main_dataSet.exe main_dataSet )
TARGET_LINK_LIBRARIES(main_dataSet.exe ${MPI_LIBRARIES} mbd)

# Second executable

ADD_EXECUTABLE(main_dataSetLevelled.exe main_dataSetLevelled )
TARGET_LINK_LIBRARIES(main_dataSetLevelled.exe ${MPI_LIBRARIES} mbd)

# Third executable

ADD_EXECUTABLE(main_eigen.exe main_eigen )
TARGET_LINK_LIBRARIES(main_eigen.exe ${MPI_LIBRARIES} mbd)