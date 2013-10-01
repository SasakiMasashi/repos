#include "mpi.h"
#include <iostream>

int main(int argc, char* argv[])
{
	int myid, numprocess;

	MPI::Init(argc, argv);

	numprocess = MPI::COMM_WORLD.Get_size();
	myid	  = MPI::COMM_WORLD.Get_rank();
	std::cout << "Process " << myid << " of " << numprocess << std::endl;

	MPI::Finalize();
	return 0;
}