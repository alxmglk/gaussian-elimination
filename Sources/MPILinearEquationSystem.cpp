#include "MPILinearEquationSystem.h"

MPILinearEquationSystem::MPILinearEquationSystem(int n, int rowsCount) : LinearEquationSystem(n, rowsCount)
{
	RowType = new MPIRowType(n);
}

MPILinearEquationSystem::~MPILinearEquationSystem()
{
	delete RowType;
}