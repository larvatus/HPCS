
DIR=/home/nicholas/University/Laurea_magistrale/Anno_II/Thesis/Anna_Fra_Nic/MBD_ECG/class

compile:
	mpicxx -o main_genton main_genton.cpp user_fun.cpp ../../combinations.cpp ../../bandDepthData.cpp ../../mpi_utility.cpp ../../dataSet.cpp -I$(DIR)

run:
	mpirun -n 4 main_genton 