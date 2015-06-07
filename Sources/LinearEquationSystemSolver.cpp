#include "LinearEquationSystemSolver.h"

LinearEquationSystemSolver::LinearEquationSystemSolver(MPIContext& mpiContext, MPICommunicator& mpiCommunicator)
	: context(mpiContext), communicator(mpiCommunicator)
{
}

void LinearEquationSystemSolver::Solve(LinearEquationSystem* system, NUMBER* solution)
{
	LinearEquationSystemSolverContext solverContext(context, system);

	ConvertToTriangularForm(solverContext);
	Backsolve(system, solution);
}

void LinearEquationSystemSolver::ConvertToTriangularForm(LinearEquationSystemSolverContext& solverContext)
{
	LinearEquationSystem* system = solverContext.System;;
	int rowsCount = system->RowsCount;
	bool* processedRows = solverContext.ProcessedRows;
	NUMBER* mainRow = solverContext.MainRow;
	int packedElementsCount = system->RowType->ElementsCount / K;

	NUMBER** matrix = system->AugmentedMatrix;

	for (int i = 0; i < system->N; ++i)
	{
		FindMainRow(solverContext, i);

		PARRAY* pMainRow = (PARRAY*)mainRow;

		for (int row = 0; row < rowsCount; ++row)
		{
			if (processedRows[row])
			{
				continue;
			}

			NUMBER multiplier = -(matrix[row][i] / mainRow[i]);

			PARRAY* currentRow = (PARRAY*)matrix[row];
			PARRAY m = SET1(multiplier);

			for (int j = 0; j < packedElementsCount; ++j)
			{
				currentRow[j] = ADD(currentRow[j], MUL(pMainRow[j], m));
			}
		}
	}
}

void LinearEquationSystemSolver::FindMainRow(LinearEquationSystemSolverContext& solverContext, int index)
{
	NUMBER** matrix = solverContext.System->AugmentedMatrix;
	int rowsCount = solverContext.System->RowsCount;
	int columnsCount = solverContext.System->ColumnsCount;
	MPI_Datatype rowType = solverContext.System->RowType->Type;
	bool* processedRows = solverContext.ProcessedRows;
	NUMBER** gatherBuffer = solverContext.GatherBuffer;
	NUMBER* mainRow = solverContext.MainRow;
	GlobalRowId* solutionMap = solverContext.SolutionMap;

	NUMBER* mainRowLocal = solverContext.DefaultMinimalRow;
	int mainRowLocalIndex = -1;

	for (int row = 0; row < rowsCount; ++row)
	{
		if (processedRows[row])
		{
			continue;
		}

		if (mainRowLocal[index] <= matrix[row][index])
		{
			mainRowLocal = matrix[row];
			mainRowLocalIndex = row;
		}
	}

	communicator.Gather(mainRowLocal, 1, gatherBuffer[0], rowType);

	int mainRowProcessRank = 0;
	if (context.IsMaster())
	{
		NUMBER maxValue = gatherBuffer[mainRowProcessRank][index];

		for (int proc = 1; proc < context.NumberOfProcesses; ++proc)
		{
			NUMBER value = gatherBuffer[proc][index];

			if (value > maxValue)
			{
				mainRowProcessRank = proc;
				maxValue = value;
			}
		}

		memcpy(mainRow, gatherBuffer[mainRowProcessRank], columnsCount * sizeof(NUMBER));
	}

	communicator.Broadcast(&mainRowProcessRank, 1, MPI_INTEGER, context.MasterProcessRank);
	if (mainRowProcessRank == context.ProcessRank)
	{
		processedRows[mainRowLocalIndex] = true;

		solutionMap[index].ProcessRank = mainRowProcessRank;
		solutionMap[index].LocalIndex = mainRowLocalIndex;
	}

	communicator.Broadcast(&solutionMap[index], 1, solverContext.GlobalRowIdType, mainRowProcessRank);

	printf("Solution map[%d]: %d + local index: %d \n", index, solutionMap[index].ProcessRank, solutionMap[index].LocalIndex);

	communicator.Broadcast(mainRow, 1, rowType, context.MasterProcessRank);
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