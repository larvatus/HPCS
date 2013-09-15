
EXECUTE_PROCESS( COMMAND ln -s ${CMAKE_CURRENT_SOURCE_DIR}/data ${CMAKE_CURRENT_BINARY_DIR}/data  )

# A first executable

ADD_EXECUTABLE(main_wilcoxon.exe main_wilcoxon.cpp )
TARGET_LINK_LIBRARIES(main_wilcoxon.exe ${MPI_LIBRARIES} mbd)
EXECUTE_PROCESS( COMMAND ln -s ${CMAKE_CURRENT_SOURCE_DIR}/weights.dat ${CMAKE_CURRENT_BINARY_DIR}/weights.w  )

ADD_EXECUTABLE(main_sample.exe main_sample.cpp )
TARGET_LINK_LIBRARIES(main_sample.exe ${MPI_LIBRARIES} mbd)

ADD_EXECUTABLE(main_mbdRefSet.exe main_mbdRefSet.cpp )
TARGET_LINK_LIBRARIES(main_mbdRefSet.exe ${MPI_LIBRARIES} mbd)