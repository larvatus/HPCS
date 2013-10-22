
EXECUTE_PROCESS( COMMAND ln -s ${CMAKE_CURRENT_SOURCE_DIR}/data ${CMAKE_CURRENT_BINARY_DIR}/data  )
EXECUTE_PROCESS( COMMAND ln -s ${CMAKE_CURRENT_SOURCE_DIR}/multiDepthMeasure.plot ${CMAKE_CURRENT_BINARY_DIR}/multiDepthMeasure.plot )
EXECUTE_PROCESS( COMMAND ln -s ${CMAKE_CURRENT_SOURCE_DIR}/depthMeasure.plot ${CMAKE_CURRENT_BINARY_DIR}/depthMeasure.plot )

# A first executable

ADD_EXECUTABLE(main_depthMeasureAll.exe main_depthMeasureAll )
TARGET_LINK_LIBRARIES(main_depthMeasureAll.exe ${MPI_LIBRARIES} HPCS)


ADD_EXECUTABLE(main_depthMeasureRef.exe main_depthMeasureRef )
TARGET_LINK_LIBRARIES(main_depthMeasureRef.exe ${MPI_LIBRARIES} HPCS)


ADD_EXECUTABLE(main_multiDepthMeasureAll.exe main_multiDepthMeasureAll )
TARGET_LINK_LIBRARIES(main_multiDepthMeasureAll.exe ${MPI_LIBRARIES} HPCS)
EXECUTE_PROCESS( COMMAND ln -s ${CMAKE_CURRENT_SOURCE_DIR}/weightsAll.dat ${CMAKE_CURRENT_BINARY_DIR}/weightsAll.w  )



ADD_EXECUTABLE(main_multiDepthMeasureRef.exe main_multiDepthMeasureRef )
TARGET_LINK_LIBRARIES(main_multiDepthMeasureRef.exe ${MPI_LIBRARIES} HPCS)
EXECUTE_PROCESS( COMMAND ln -s ${CMAKE_CURRENT_SOURCE_DIR}/weightsRef.dat ${CMAKE_CURRENT_BINARY_DIR}/weightsRef.w  )