
compile:
	I AM A BULLISH (AND WRONG) INSERTION!!!

	mpicxx -o main main.cpp bandDepth.cpp dataSet.cpp combinationFactory.cpp mpi_utility.cpp bandDepthRef.cpp

run:
	mpirun -n 4 main
