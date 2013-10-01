#include "mpi.h"
#include "stdio.h"
#include <iostream>
#include <fstream>
#include <winsock.h>

int main(int argc, char* argv[])
{
	int myid, numprocess;

	MPI::Init(argc, argv);

	numprocess = MPI::COMM_WORLD.Get_size();
	myid = MPI::COMM_WORLD.Get_rank();

	MPI_Status status;
	int tag = 0;
	char message[256];

	char szBuf[256], szIP[256];
	gethostname(szBuf, (int)sizeof(szBuf));
	
	HOSTENT *lpHost = gethostbyname(szBuf);
	IN_ADDR inaddr;
	memcpy(&inaddr, lpHost->h_addr_list[0], 4);
	strcpy(szIP, inet_ntoa(inaddr));	

	if(myid != 0)
	{
		int dest = 0;
		sprintf(message, "Greetings from process %d to process %d, hostname:%s, ipaddress:%s", myid, dest, szBuf, szIP);
		MPI_Send(message, strlen(message) + 1, MPI_CHAR, dest, tag, MPI_COMM_WORLD);
	}
	else
	{
		std::ofstream fout("C:\\gomi\\MPICH2Test2_result.txt");
		sprintf(message, "process %d, hostname:%s, ipaddress:%s", myid, szBuf, szIP);
		fout << message << std::endl;

		for(int source = 1; source < numprocess; source++)
		{
			MPI_Recv(message, 256, MPI_CHAR, source, tag, MPI_COMM_WORLD, &status);			
			fout << message << std::endl;
		}
	}

	MPI::Finalize();
}

