#pragma once
#include "MPIRowType.h"
#include "intrin.h"

class LinearEquationSystem
{
public:
	MPIRowType* RowType;
	NUMBER** AugmentedMatrix;
	int N;
	int RowsCount;
	int ColumnsCount;
	int FreeTermIndex;

	LinearEquationSystem(int n);
	LinearEquationSystem(int n, int rowsCount);
	~LinearEquationSystem();
};