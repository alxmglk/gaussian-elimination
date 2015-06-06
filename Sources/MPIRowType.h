#pragma once
#include "Common.h"
#include "mpi.h"

class MPIRowType
{
public:
	int Size;
	MPI_Datatype Type;

	MPIRowType(int n);
	~MPIRowType();
};