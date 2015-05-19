#pragma once
#include "Common.h"
#include "LinearEquationSystem.h"
#include <math.h>
#include <intrin.h>
#include <omp.h>

class LinearEquationSystemSolver
{
public:
	void Solve(LinearEquationSystem* system, NUMBER* solution);
	void ConvertToTriangularForm(LinearEquationSystem* system);
	void Backsolve(LinearEquationSystem* system, NUMBER* solution);
};