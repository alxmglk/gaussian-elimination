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

	_globalRowIdType = new MPIGlobalRowIdType();
	GlobalRowIdType = _globalRowIdType->Type;

	MainRow = (NUMBER*)_aligned_malloc(system->RowType->Size, 16);
	MainRows = new GlobalRowId[system->N];
}

LinearEquationSystemSolverContext::~LinearEquationSystemSolverContext()
{
	_aligned_free(MainRow);
	delete[]DefaultMinimalRow;
	delete[]ProcessedRows;
	delete[]MainRows;
	delete _gatherBuffer;
	delete _globalRowIdType;
}