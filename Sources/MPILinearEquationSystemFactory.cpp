#include "MPILinearEquationSystemFactory.h"

MPILinearEquationSystemFactory::MPILinearEquationSystemFactory(LinearEquationSystemFactory* defaultFactory, MPIContext* mpiContext, MPICommunicator* mpiCommunicator)
{
	factory = defaultFactory;
	context = mpiContext;
	communicator = mpiCommunicator;
}

MPILinearEquationSystem* MPILinearEquationSystemFactory::Create(int n)
{
	int rowsPerProcess = n / context->NumberOfProcesses;
	MPILinearEquationSystem* partialSystem = new MPILinearEquationSystem(n, rowsPerProcess);
	LinearEquationSystem* fullSystem;

	if (context->IsMaster())
	{
		fullSystem = factory->Create(n);
	}
	else
	{
		fullSystem = new LinearEquationSystem(n);
	}

	communicator->Scatter(fullSystem->AugmentedMatrix[0], rowsPerProcess, partialSystem->AugmentedMatrix[0], partialSystem->RowType->Type);

	delete fullSystem;

	return partialSystem;
}