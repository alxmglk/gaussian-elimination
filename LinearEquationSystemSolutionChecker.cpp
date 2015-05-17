#include "LinearEquationSystemSolutionChecker.h"

bool LinearEquationSystemSolutionChecker::IsCorrectSolution(LinearEquationSystem* system, float* solution)
{
	float precision = 0.1f;
	bool correct = true;

	float** matrix = system->AugmentedMatrix;
	int freeTermColumn = system->ColumnsCount - 1;

	for (int row = 0; row < system->RowsCount; ++row)
	{
		float freeTerm = matrix[row][freeTermColumn];

		for (int column = 0; column < freeTermColumn; ++column)
		{
			freeTerm -= matrix[row][column] * solution[column];
		}

		if (abs(freeTerm) > precision)
		{
 			correct = false;
			break;
		}
	}

	return correct;
}