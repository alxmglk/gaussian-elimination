#include "LinearEquationSystemSolver.h"

LinearEquationSystemSolver::LinearEquationSystemSolver(MPIContext& mpiContext, MPICommunicator& mpiCommunicator) 
	: context(mpiContext), communicator(mpiCommunicator)
{	
}

void LinearEquationSystemSolver::Solve(LinearEquationSystem* system, NUMBER* solution)
{
	ConvertToTriangularForm(system);
	Backsolve(system, solution);
}

void LinearEquationSystemSolver::ConvertToTriangularForm(LinearEquationSystem* system)
{
	int n = system->N;
	MPI_Datatype rowType = system->RowType->Type;
	int rowsCount = system->RowsCount;
	int columnsCount = system->ColumnsCount;

	bool* processedRows = new bool[rowsCount];
	for (int k = 0; k < rowsCount; ++k)
	{
		processedRows[k] = false;
	}

	NUMBER* multiplierRow = (NUMBER*)_aligned_malloc(system->RowType->Size, 16);
	NUMBER* emptyRow = new NUMBER[columnsCount];

	for (int column = 0; column < columnsCount; ++column)
	{
		emptyRow[column] = -FLT_MAX;
	}

	LinearEquationSystem* gatherBuffer = new LinearEquationSystem(n, context.NumberOfProcesses);

	NUMBER** matrix = system->AugmentedMatrix;

	for (int i = 0; i < n - 1; ++i)
	{
#pragma region Find row with the largest coefficient
		NUMBER* mainRow = emptyRow;

		int mainRowIndex = -1;

		for (int row = 0; row < rowsCount; ++row)
		{
			if (processedRows[row])
			{
				continue;
			}

			if (mainRow[i] <= matrix[row][i])
			{
				mainRow = matrix[row];
				mainRowIndex = row;
			}
		}

		communicator.Gather(mainRow, 1, gatherBuffer->AugmentedMatrix[0], rowType);

		int maxValueProc = 0;
		if (context.IsMaster())
		{
			NUMBER maxValue = gatherBuffer->AugmentedMatrix[maxValueProc][i];

			for (int proc = 1; proc < context.NumberOfProcesses; ++proc)
			{
				NUMBER value = gatherBuffer->AugmentedMatrix[proc][i];

				if (value > maxValue)
				{
					maxValueProc = proc;
					maxValue = value;
				}
			}

			memcpy(multiplierRow, gatherBuffer->AugmentedMatrix[maxValueProc], columnsCount * sizeof(NUMBER));
		}
#pragma endregion

		communicator.Broadcast(&maxValueProc, 1, MPI_INTEGER, context.MasterProcessRank);
		if (maxValueProc == context.ProcessRank)
		{
			processedRows[mainRowIndex] = true;
		}

		communicator.Broadcast(multiplierRow, 1, rowType, context.MasterProcessRank);

#pragma region Solve
		int packedElementsNumber = columnsCount / K;

		PARRAY* pMultiplierRow = (PARRAY*)multiplierRow;

		for (int row = 0; row < rowsCount; ++row)
		{
			if (processedRows[row])
			{
				continue;
			}

			NUMBER multiplier = -(matrix[row][i] / multiplierRow[i]);

			PARRAY* currentRow = (PARRAY*)matrix[row];
			PARRAY m = SET1(multiplier);

			for (int j = 0; j < packedElementsNumber; ++j)
			{
				currentRow[j] = ADD(currentRow[j], MUL(pMultiplierRow[j], m));
			}
		}
#pragma endregion
	}

	_aligned_free(multiplierRow);
	delete[]processedRows;
	delete[]emptyRow;
	delete gatherBuffer;
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