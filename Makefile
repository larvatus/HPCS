
compile:
	
	mpicxx -o main main.cpp bandDepth.cpp dataSet.cpp combinationFactory.cpp mpi_utility.cpp

run:
	mpirun -n 4 main
