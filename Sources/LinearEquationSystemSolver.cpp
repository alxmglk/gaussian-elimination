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

	for (int i = 0; i < system->N - 1; ++i)
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

	NUMBER* mainLocalRow = solverContext.DefaultMinimalRow;
	int mainLocalRowIndex = -1;

	for (int row = 0; row < rowsCount; ++row)
	{
		if (processedRows[row])
		{
			continue;
		}

		if (mainLocalRow[index] <= matrix[row][index])
		{
			mainLocalRow = matrix[row];
			mainLocalRowIndex = row;
		}
	}

	communicator.Gather(mainLocalRow, 1, gatherBuffer[0], rowType);

	int maxValueProc = 0;
	if (context.IsMaster())
	{
		NUMBER maxValue = gatherBuffer[maxValueProc][index];

		for (int proc = 1; proc < context.NumberOfProcesses; ++proc)
		{
			NUMBER value = gatherBuffer[proc][index];

			if (value > maxValue)
			{
				maxValueProc = proc;
				maxValue = value;
			}
		}

		memcpy(mainRow, gatherBuffer[maxValueProc], columnsCount * sizeof(NUMBER));
	}

	communicator.Broadcast(&maxValueProc, 1, MPI_INTEGER, context.MasterProcessRank);
	if (maxValueProc == context.ProcessRank)
	{
		processedRows[mainLocalRowIndex] = true;
	}

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