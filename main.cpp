#include <stdio.h>
#include <math.h>
#include <windows.h>
#include <intrin.h>

#include "Stopwatch.h"

Stopwatch stopwatch;
const int n = 3;

void generateLinearEquationSystem(float** augmentedMatrix, int rowsCount, int rowLength);
void convertToTriangularForm(float** matrix, int rowsCount, int rowLength);
void backsolve(float** matrix, int rowsCount, int rowLength, float* solution);
void print(float* matrix, int length);

int main()
{
	float m[3][4] = {
			{ 2, 1, -1, 8 },
			{ -3, -1, 2, -11 },
			{ -2, 1, 2, -3 }
	};

	float* m2[4] = {
		m[0], m[1], m[2]
	};

	int rowLength = n + 1;
	int rowsCount = n;

	float* solution = new float[n];

	// 1 generate random matrix
	stopwatch.Start();

	convertToTriangularForm(m2, 3, 4);
	backsolve(m2, 3, 4, solution);

	stopwatch.Stop();
	stopwatch.ShowElapsedTime();

	print(solution, n);

	delete[] solution;

	system("pause");
}

void generateLinearEquationSystem(float** matrix, int rowsCount, int columnsCount)
{
	int maxValue = 42;
	float divider = 3.33f;

	for (int row = 0; row < rowsCount; ++row)
	{
		float freeTerm = 0;

		int column = 0;
		for (; column < columnsCount - 1; ++column)
		{
			float value = (rand() % maxValue - (maxValue / 2)) / divider;

			matrix[row][column] = value;
			freeTerm += value * column;
		}

		matrix[row][column] = freeTerm;
	}
}

void convertToTriangularForm(float** matrix, int rowsCount, int rowLength)
{
	float precision = 0.000000001f;

	for (int row = 0; row < rowsCount - 1; ++row)
	{
		int column = row;

		if (matrix[row][column] < precision)
		{
			// find row with non-zero coefficient in current column.
		}

		for (int i = row + 1; i < rowsCount; ++i)
		{
			float multiplier = -(matrix[i][column] / matrix[row][column]);

			for (int j = 0; j < rowLength; ++j)
			{
				matrix[i][j] += matrix[row][j] * multiplier;
			}
		}
	}
}

void backsolve(float** matrix, int rowsCount, int rowLength, float* solution)
{
	int n = rowsCount - 1;
	int freeTermIndex = n + 1;
		
	int rowIndex = n;
	float* row = matrix[n];

	for (; rowIndex >= 0; --rowIndex, row = matrix[rowIndex])
	{
		float freeTerm = row[freeTermIndex];
		int columnIndex = n;

		for (; columnIndex > rowIndex; --columnIndex)
		{
			freeTerm -= solution[columnIndex] * row[columnIndex];
		}

		solution[rowIndex] = freeTerm / row[columnIndex];
	}
}

void print(float* matrix, int length)
{
	for (int i = 0; i < length; ++i)
	{
		printf("%f ", matrix[i]);
	}

	printf("\n");
}