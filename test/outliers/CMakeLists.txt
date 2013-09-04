

EXECUTE_PROCESS( COMMAND ln -s ${CMAKE_CURRENT_SOURCE_DIR}/data ${CMAKE_CURRENT_BINARY_DIR}/data  )
EXECUTE_PROCESS( COMMAND ln -s ${CMAKE_CURRENT_SOURCE_DIR}/outliers.plot ${CMAKE_CURRENT_BINARY_DIR}/outliers.plot )

# A first executable

ADD_EXECUTABLE(main_outliersAll.exe main_outliersAll )
TARGET_LINK_LIBRARIES(main_outliersAll.exe ${MPI_LIBRARIES} mbd)
EXECUTE_PROCESS( COMMAND ln -s ${CMAKE_CURRENT_SOURCE_DIR}/weightsAll.dat ${CMAKE_CURRENT_BINARY_DIR}/weightsAll.w  )


ADD_EXECUTABLE(main_outliersRef.exe main_outliersRef )
TARGET_LINK_LIBRARIES(main_outliersRef.exe ${MPI_LIBRARIES} mbd)
EXECUTE_PROCESS( COMMAND ln -s ${CMAKE_CURRENT_SOURCE_DIR}/weightsRef.dat ${CMAKE_CURRENT_BINARY_DIR}/weightsRef.w)
