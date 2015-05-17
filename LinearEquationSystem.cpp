#include "LinearEquationSystem.h"

LinearEquationSystem::LinearEquationSystem(int n)
{
	N = n;
	RowsCount = n;
	ColumnsCount = n + 1;
	FreeTermIndex = n;
	AugmentedMatrix = (float**)malloc(RowsCount * sizeof(float*));

	for (int row = 0; row < RowsCount; ++row)
	{
		AugmentedMatrix[row] = (float*)malloc(ColumnsCount * sizeof(float));
	}
}

LinearEquationSystem::~LinearEquationSystem()
{
	for (int row = 0; row < RowsCount; ++row)
	{
		free(AugmentedMatrix[row]);
	}

	free(AugmentedMatrix);
}