#pragma once
#include "LinearEquationSystemFactory.h"
#include "MPIContext.h"
#include "MPICommunicator.h"
#include "MPILinearEquationSystem.h"

class MPILinearEquationSystemFactory
{
	LinearEquationSystemFactory* factory;
	MPIContext* context;
	MPICommunicator* communicator;

public:
	MPILinearEquationSystemFactory(LinearEquationSystemFactory* defaultFactory, MPIContext* mpiContext, MPICommunicator* mpiCommunicator);
	
    MPILinearEquationSystem* Create(int n);
};