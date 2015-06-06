#include "MPIRowType.h"

MPIRowType::MPIRowType(int n)
{
	Size = n + 1;

	MPI_Type_contiguous(Size, MPI_NUMBER, &Type);
	MPI_Type_commit(&Type);
}

MPIRowType::~MPIRowType()
{
	MPI_Type_free(&Type);
}