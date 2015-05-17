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
	int FreeTermIndex;

public:
	LinearEquationSystem(int n);
	~LinearEquationSystem();
};