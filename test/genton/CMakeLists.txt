


# A first executable

ADD_EXECUTABLE(main_genton.exe main_genton user_fun )
TARGET_LINK_LIBRARIES(main_genton.exe ${MPI_LIBRARIES} mbd)
# ENABLE_TESTING()
# ADD_TEST(mytest  ${MPIEXEC} ${MPIEXEC_NUMPROC_FLAG} 4 ${CMAKE_CURRENT_BINARY_DIR}/mainDataSet.exe )
EXECUTE_PROCESS( COMMAND ln -s ${CMAKE_CURRENT_SOURCE_DIR}/data ${CMAKE_CURRENT_BINARY_DIR}/data  )
