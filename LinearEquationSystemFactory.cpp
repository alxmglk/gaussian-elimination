#include "LinearEquationSystemFactory.h"

LinearEquationSystem* LinearEquationSystemFactory::Create(int n)
{
	int maxValue = 42;
	float divider = 3.33f;

	LinearEquationSystem* system = new LinearEquationSystem(n);
	float** matrix = system->AugmentedMatrix;

	for (int row = 0; row < system->RowsCount; ++row)
	{
		float freeTerm = 0;

		int column = 0;
		for (; column < system->ColumnsCount - 1; ++column)
		{
			float value = (rand() % maxValue - (maxValue / 2)) / divider;

			matrix[row][column] = value;
			freeTerm += value * column;
		}

		matrix[row][column] = freeTerm;
	}

	return system;
}