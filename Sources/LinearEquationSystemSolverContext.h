#pragma once
#include "mpi.h"
#include "LinearEquationSystem.h"
#include "MPIContext.h"
#include <float.h>

class LinearEquationSystemSolverContext
{
	MPIContext& context;
	LinearEquationSystem* _gatherBuffer;
	
public:
	LinearEquationSystem* System;
	NUMBER* MainRow;
	NUMBER* DefaultMinimalRow;
	NUMBER** GatherBuffer;	
	bool* ProcessedRows;

	LinearEquationSystemSolverContext(MPIContext& mpiContext, LinearEquationSystem* system);
	~LinearEquationSystemSolverContext();
};