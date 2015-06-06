#pragma once
#include "MPIContext.h"
#include "MPICommunicator.h"
#include "LinearEquationSystem.h"
#include <time.h>
#include <windows.h>

class LinearEquationSystemFactory
{
	MPIContext& context;
	MPICommunicator& communicator;

public:
	LinearEquationSystemFactory(MPIContext& mpiContext, MPICommunicator& mpiCommunicator);
	
    LinearEquationSystem* Create(int n);
	void Fill(LinearEquationSystem* system);
};