#pragma once
#include <stdlib.h>
#include "Common.h"

class LinearEquationSystem
{
public:
	NUMBER** AugmentedMatrix;
	int N;
	int RowsCount;
	int ColumnsCount;
	int ColumnsCountWithBuffer;
	int FreeTermIndex;

	LinearEquationSystem(int n);
	~LinearEquationSystem();
};