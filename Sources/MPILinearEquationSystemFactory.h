#pragma once
#include "LinearEquationSystemFactory.h"
#include "MPIContext.h"
#include "MPICommunicator.h"
#include "MPIRowType.h"

class MPILinearEquationSystemFactory : public LinearEquationSystemFactory
{
	MPIRowType* rowType;
	MPIContext* context;
	MPICommunicator* communicator;

public:
	MPILinearEquationSystemFactory(MPIRowType* mpiRowType, MPIContext* mpiContext, MPICommunicator* mpiCommunicator);
	virtual ~MPILinearEquationSystemFactory();

	virtual LinearEquationSystem* Create(int n) override;
};