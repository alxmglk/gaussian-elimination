#pragma once
#include <stdlib.h>

class LinearEquationSystem
{
public:
	float** AugmentedMatrix;
	int N;
	int RowsCount;
	int ColumnsCount;
	int FreeTermIndex;

public:
	LinearEquationSystem(int n);
	~LinearEquationSystem();
};