#pragma once
#include <math.h>
#include "LinearEquationSystem.h"

class LinearEquationSystemSolutionChecker
{
public:
	bool IsCorrectSolution(LinearEquationSystem* system, float* solution);
};