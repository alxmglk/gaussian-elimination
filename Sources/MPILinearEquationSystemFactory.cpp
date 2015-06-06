#include "MPILinearEquationSystemFactory.h"

MPILinearEquationSystemFactory::MPILinearEquationSystemFactory(MPIRowType* mpiRowType, MPIContext* mpiContext, MPICommunicator* mpiCommunicator)
{
	rowType = mpiRowType;
	context = mpiContext;
	communicator = mpiCommunicator;
}

MPILinearEquationSystemFactory::~MPILinearEquationSystemFactory()
{
}

LinearEquationSystem* MPILinearEquationSystemFactory::Create(int n)
{
	int rowsPerProcess = n / context->NumberOfProcesses;
	LinearEquationSystem* partialSystem = new LinearEquationSystem(n, rowsPerProcess);
	LinearEquationSystem* fullSystem;

	if (context->IsMaster())
	{
		fullSystem = LinearEquationSystemFactory::Create(n);
	}
	else
	{
		fullSystem = new LinearEquationSystem(n);
	}

	communicator->Scatter(fullSystem->AugmentedMatrix[0], rowsPerProcess, partialSystem->AugmentedMatrix[0], rowType->Type);

	delete fullSystem;

	return partialSystem;
}