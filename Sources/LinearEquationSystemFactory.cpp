#include "LinearEquationSystemFactory.h"

LinearEquationSystemFactory::~LinearEquationSystemFactory()
{
}

LinearEquationSystem* LinearEquationSystemFactory::Create(int n)
{
	int maxValue = 42;
	NUMBER divider = 3.33f;

	LinearEquationSystem* system = new LinearEquationSystem(n);
	NUMBER** matrix = system->AugmentedMatrix;

	srand(time(nullptr));

	for (int row = 0; row < system->RowsCount; ++row)
	{
		NUMBER freeTerm = 0;

		int column = 0;
		for (; column < system->ColumnsCount - 1; ++column)
		{
			NUMBER value = (rand() % maxValue - (maxValue / 2)) / divider;

			matrix[row][column] = value;
			freeTerm += value * column;
		}

		matrix[row][column] = freeTerm;
	}

	return system;
}