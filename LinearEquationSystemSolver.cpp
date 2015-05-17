#include "LinearEquationSystemSolver.h"

void LinearEquationSystemSolver::Solve(LinearEquationSystem* system, float* solution)
{
	ConvertToTriangularForm(system);
	Backsolve(system, solution);
}

void LinearEquationSystemSolver::ConvertToTriangularForm(LinearEquationSystem* system)
{
	float precision = 0.000000001f;

	int rowsCount = system->RowsCount;
	int columnsCount = system->ColumnsCount;
	float** matrix = system->AugmentedMatrix;

	for (int row = 0; row < rowsCount - 1; ++row)
	{
		int column = row;

		if (abs(matrix[row][column]) < precision)
		{
			for (int i = row + 1; i < rowsCount; ++i)
			{
				if (matrix[i][column] > precision)
				{
					for (int j = 0; j < rowsCount; ++j)
					{
						float temp = matrix[row][j];
						matrix[row][j] = matrix[i][j];
						matrix[i][j] = temp;
					}

					break;
				}
			}
		}

		for (int i = row + 1; i < rowsCount; ++i)
		{
			float multiplier = -(matrix[i][column] / matrix[row][column]);

			for (int j = 0; j < columnsCount; ++j)
			{
				matrix[i][j] += matrix[row][j] * multiplier;
			}
		}
	}
}

void LinearEquationSystemSolver::Backsolve(LinearEquationSystem* system, float* solution)
{
	int lastRowIndex = system->RowsCount - 1;
	int freeTermIndex = system->FreeTermIndex;
	float** matrix = system->AugmentedMatrix;

	int rowIndex = lastRowIndex;
	float* row = matrix[lastRowIndex];

	for (; rowIndex >= 0; --rowIndex, row = matrix[rowIndex])
	{
		float freeTerm = row[freeTermIndex];
		int columnIndex = lastRowIndex;

		for (; columnIndex > rowIndex; --columnIndex)
		{
			freeTerm -= solution[columnIndex] * row[columnIndex];
		}

		solution[rowIndex] = freeTerm / row[columnIndex];
	}
}