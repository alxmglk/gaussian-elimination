#include "LinearEquationSystemSolver.h"

void LinearEquationSystemSolver::Solve(LinearEquationSystem* system, NUMBER* solution)
{
	ConvertToTriangularForm(system);
	Backsolve(system, solution);
}

void LinearEquationSystemSolver::ConvertToTriangularForm(LinearEquationSystem* system)
{
	NUMBER precision = 0.000000001f;

	int rowsCount = system->RowsCount;
	int columnsCount = system->ColumnsCount;
	NUMBER** matrix = system->AugmentedMatrix;

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
						NUMBER temp = matrix[row][j];
						matrix[row][j] = matrix[i][j];
						matrix[i][j] = temp;
					}

					break;
				}
			}
		}

		#pragma omp parallel num_threads(8) if (rowsCount - row > 128) 
		{
			#pragma omp for nowait
			for (int i = row + 1; i < rowsCount; ++i)
			{
				NUMBER multiplier = -(matrix[i][column] / matrix[row][column]);

				for (int j = 0; j < columnsCount; ++j)
				{
					matrix[i][j] += matrix[row][j] * multiplier;
				}
			}
		}
	}
}

void LinearEquationSystemSolver::Backsolve(LinearEquationSystem* system, NUMBER* solution)
{
	int lastRowIndex = system->RowsCount - 1;
	int freeTermIndex = system->FreeTermIndex;
	NUMBER** matrix = system->AugmentedMatrix;

	int rowIndex = lastRowIndex;
	NUMBER* row = matrix[lastRowIndex];

	for (; rowIndex >= 0; --rowIndex, row = matrix[rowIndex])
	{
		NUMBER freeTerm = row[freeTermIndex];
		int columnIndex = lastRowIndex;

		for (; columnIndex > rowIndex; --columnIndex)
		{
			freeTerm -= solution[columnIndex] * row[columnIndex];
		}

		solution[rowIndex] = freeTerm / row[columnIndex];
	}
}