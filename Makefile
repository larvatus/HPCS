
compile:
	
	mpicxx -o main.exe main.cpp dataSet.cpp bandDepthData.cpp bandDepth.cpp mpi_utility.cpp bandDepthRef.cpp combinationFactory.cpp

run:
	mpirun -n 4 main.exe
