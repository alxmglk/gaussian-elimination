#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "mpi.h"
#include "GlobalRowId.h"

class MPIGlobalRowIdType
{
public:
	MPI_Datatype Type;

	MPIGlobalRowIdType();
	~MPIGlobalRowIdType();
};