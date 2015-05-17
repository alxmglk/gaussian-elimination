#include "LinearEquationSystem.h"

LinearEquationSystem::LinearEquationSystem(int n)
{
	N = n;
	RowsCount = n;
	ColumnsCount = n + 1;
	FreeTermIndex = n;
	AugmentedMatrix = (NUMBER**)malloc(RowsCount * sizeof(NUMBER*));

	for (int row = 0; row < RowsCount; ++row)
	{
		AugmentedMatrix[row] = (NUMBER*)_aligned_malloc(ColumnsCount * sizeof(NUMBER), 16);
	}
}

LinearEquationSystem::~LinearEquationSystem()
{
	for (int row = 0; row < RowsCount; ++row)
	{
		_aligned_free(AugmentedMatrix[row]);
	}

	free(AugmentedMatrix);
}