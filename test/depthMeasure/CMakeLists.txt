
EXECUTE_PROCESS( COMMAND ln -s ${CMAKE_CURRENT_SOURCE_DIR}/data ${CMAKE_CURRENT_BINARY_DIR}/data  )

# A first executable

ADD_EXECUTABLE(main_depthMeasureAll.exe main_depthMeasureAll )
TARGET_LINK_LIBRARIES(main_depthMeasureAll.exe ${MPI_LIBRARIES} mbd)
# ENABLE_TESTING()
# ADD_TEST(mytest  ${MPIEXEC} ${MPIEXEC_NUMPROC_FLAG} 4 ${CMAKE_CURRENT_BINARY_DIR}/mainDataSet.exe )

ADD_EXECUTABLE(main_depthMeasureRef.exe main_depthMeasureRef )
TARGET_LINK_LIBRARIES(main_depthMeasureRef.exe ${MPI_LIBRARIES} mbd)


ADD_EXECUTABLE(main_multiDepthMeasureAll.exe main_multiDepthMeasureAll )
TARGET_LINK_LIBRARIES(main_multiDepthMeasureAll.exe ${MPI_LIBRARIES} mbd)
EXECUTE_PROCESS( COMMAND ln -s ${CMAKE_CURRENT_SOURCE_DIR}/weightsAll.dat ${CMAKE_CURRENT_BINARY_DIR}/weightsAll.dat  )
EXECUTE_PROCESS( COMMAND ln -s ${CMAKE_CURRENT_SOURCE_DIR}/multiDepthMeasureAll.plot ${CMAKE_CURRENT_BINARY_DIR}/multiDepthMeasureAll.plot )


# ADD_EXECUTABLE(main_depthMeasureRef.exe main_depthMeasureRef )
# TARGET_LINK_LIBRARIES(main_depthMeasureRef.exe ${MPI_LIBRARIES} mbd)

EXECUTE_PROCESS( COMMAND ln -s ${CMAKE_CURRENT_SOURCE_DIR}/weightsRef.dat ${CMAKE_CURRENT_BINARY_DIR}/weightsRef.dat  )