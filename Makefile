
compile:
	
# 	mpicxx -o main.exe main.cpp dataSet.cpp bandDepthData.cpp bandDepth.cpp mpi_utility.cpp bandDepthRef.cpp combinations.cpp
	mpicxx -o main.exe main.cpp dataSet.cpp bandDepthData.cpp mpi_utility.cpp combinations.cpp

run:
	mpirun -n 4 main.exe
