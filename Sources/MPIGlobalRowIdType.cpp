#include "MPIGlobalRowIdType.h"

MPIGlobalRowIdType::MPIGlobalRowIdType()
{
	int blockLengths[2] = { 1, 1 };
	MPI_Datatype types[2] = { MPI_INT, MPI_INT };
	MPI_Aint offsets[2];

	offsets[0] = offsetof(GlobalRowId, ProcessRank);
	offsets[1] = offsetof(GlobalRowId, LocalIndex);

	MPI_Type_create_struct(2, blockLengths, offsets, types, &Type);
	MPI_Type_commit(&Type);
}

MPIGlobalRowIdType::~MPIGlobalRowIdType()
{
	MPI_Type_free(&Type);
}