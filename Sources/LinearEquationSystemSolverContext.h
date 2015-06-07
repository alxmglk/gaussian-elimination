#pragma once
#include "mpi.h"
#include "LinearEquationSystem.h"
#include "MPIContext.h"
#include "MPIGlobalRowIdType.h"
#include <float.h>

class LinearEquationSystemSolverContext
{
	MPIContext& context;
	LinearEquationSystem* _gatherBuffer;
	MPIGlobalRowIdType* _globalRowIdType;

public:
	LinearEquationSystem* System;
	NUMBER* MainRow;
	NUMBER* DefaultMinimalRow;
	NUMBER** GatherBuffer;
	bool* ProcessedRows;
	GlobalRowId* SolutionMap;
	MPI_Datatype GlobalRowIdType;

	LinearEquationSystemSolverContext(MPIContext& mpiContext, LinearEquationSystem* system);
	~LinearEquationSystemSolverContext();
};