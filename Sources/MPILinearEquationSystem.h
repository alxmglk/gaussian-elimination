#pragma once
#include "LinearEquationSystem.h"
#include "MPIRowType.h"

class MPILinearEquationSystem: public LinearEquationSystem
{
public:
	MPIRowType* RowType;

	MPILinearEquationSystem(int n, int rowsCount);
	~MPILinearEquationSystem();
};