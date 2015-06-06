#pragma once
#include "mpi.h"
#include "MPIContext.h"

class MPICommunicator
{
	MPI_Comm id;
	const MPIContext& context;

public:
	MPICommunicator(const MPIContext& mpiContext, MPI_Comm communicatorId);

	void Scatter(const void* sendBuffer, int sendCount, void* recvBuffer, MPI_Datatype sendType);
	void Gather(const void* sendBuffer, int sendCount, void* recvBuffer, MPI_Datatype sendType);
	void Broadcast(void* buffer, int count, MPI_Datatype type, int root);
};