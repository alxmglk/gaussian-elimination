#include <stdio.h>
#include <math.h>
#include <windows.h>
#include <intrin.h>

#include "Stopwatch.h"

Stopwatch stopwatch;
const int n = 3;

void generateLinearEquationSystem(float** augmentedMatrix, int rowsCount, int rowLength);
void convertToTriangularForm(float** matrix, int rowsCount, int rowLength);

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

	float* arr = new float[rowsCount * rowLength];

	// 1 generate random matrix
	stopwatch.Start();

	convertToTriangularForm(m2, 3, 4);
	
	stopwatch.Stop();
	stopwatch.ShowElapsedTime();

	delete[] arr;

	system("pause");
}

void swapRows(float* matrix, int rowLength, int firstRow, int secondRow)
{
	int firstIndex = rowLength * firstRow;
	int secondIndex = rowLength * secondRow;

	for (int i = 0; i < rowLength; ++i, ++firstIndex, ++secondIndex)
	{
		float temp = matrix[firstIndex];
		matrix[firstIndex] = matrix[secondIndex];
		matrix[secondIndex] = temp;
	}
}

void multiplyRow(float* matrix, int rowLength, int row, float multiplier)
{
	int index = rowLength * row;

	for (int i = 0; i < rowLength; ++i, ++index)
	{
		matrix[index] *= multiplier;
	}
}

void addRows(float* matrix, int rowLength, int targetRow, int sourceRow)
{
	int targetIndex = rowLength * targetRow;
	int sourceIndex = rowLength * targetRow;

	for (int i = 0; i < rowLength; ++i, ++targetIndex, ++sourceIndex)
	{
		matrix[targetIndex] += matrix[sourceIndex];
	}
}

void generateLinearEquationSystem(float** matrix, int rowsCount, int columnsCount)
{
	int maxValue = 42;
	float divider = 3.33;

	for (int row = 0; row < rowsCount; ++row)
	{
		float freeTerm = 0;
		
		int column = 0;
		for (; column < columnsCount - 1; ++column)
		{
			float value = (rand() % maxValue - (maxValue / 2)) / divider;

			matrix[row][column] = value;
			freeTerm += value * j;
		}

		matrix[row][column] = freeTerm;
	}
}

void convertToTriangularForm(float** matrix, int rowsCount, int rowLength)
{
	float precision = 0.000000001;

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