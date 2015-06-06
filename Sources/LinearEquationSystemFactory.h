#pragma once
#include <time.h>
#include <windows.h>
#include "LinearEquationSystem.h"

class LinearEquationSystemFactory
{
protected:
	~LinearEquationSystemFactory();

public:
	virtual LinearEquationSystem* Create(int n);
};