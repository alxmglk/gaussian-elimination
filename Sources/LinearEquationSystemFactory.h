#pragma once
#include <time.h>
#include <windows.h>
#include "LinearEquationSystem.h"

class LinearEquationSystemFactory
{
public:
	LinearEquationSystem* Create(int n);
};