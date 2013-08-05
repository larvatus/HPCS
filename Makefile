
compile:
	
	mpicxx -o main main.cpp bandDepth.cpp dataSet.cpp combinationFactory.cpp

run:
	mpirun -n 4 main