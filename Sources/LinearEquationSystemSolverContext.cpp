#include "LinearEquationSystemSolverContext.h"

LinearEquationSystemSolverContext::LinearEquationSystemSolverContext(MPIContext& mpiContext, LinearEquationSystem* system)
	: context(mpiContext)
{
	System = system;
	
	DefaultMinimalRow = new NUMBER[system->ColumnsCount];
	for (int column = 0; column < system->ColumnsCount; ++column)
	{
		DefaultMinimalRow[column] = -FLT_MAX;
	}

	ProcessedRows = new bool[system->RowsCount];
	for (int row = 0; row < system->RowsCount; ++row)
	{
		ProcessedRows[row] = false;
	}	

	_gatherBuffer = new LinearEquationSystem(system->N, context.NumberOfProcesses);
	GatherBuffer = _gatherBuffer->AugmentedMatrix;
	MainRow = (NUMBER*)_aligned_malloc(system->RowType->Size, 16);
}

LinearEquationSystemSolverContext::~LinearEquationSystemSolverContext()
{
	_aligned_free(MainRow);
	delete[]DefaultMinimalRow;
	delete[]ProcessedRows;
	delete _gatherBuffer;
}