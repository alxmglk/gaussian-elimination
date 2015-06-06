#pragma once
#include "Common.h"
#include "LinearEquationSystem.h"
#include "LinearEquationSystemSolverContext.h"
#include "MPIContext.h"
#include "MPICommunicator.h"
#include <math.h>
#include <intrin.h>
#include <float.h>
#include <windows.h>
#include <stdio.h>

class LinearEquationSystemSolver
{
	MPIContext& context;
	MPICommunicator& communicator;

	void ConvertToTriangularForm(LinearEquationSystemSolverContext& solverContext);
	void FindMainRow(LinearEquationSystemSolverContext& solverContext, int index);
	void Backsolve(LinearEquationSystem* system, NUMBER* solution);

public:
	LinearEquationSystemSolver(MPIContext& mpiContext, MPICommunicator& mpiCommunicator);

	void Solve(LinearEquationSystem* system, NUMBER* solution);
};