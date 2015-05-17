#pragma once
#include "LinearEquationSystem.h"
#include <math.h>

class LinearEquationSystemSolver
{
public:
	void Solve(LinearEquationSystem* system, float* solution);
	void ConvertToTriangularForm(LinearEquationSystem* system);
	void Backsolve(LinearEquationSystem* system, float* solution);
};