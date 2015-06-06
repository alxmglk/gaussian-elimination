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
	LinearEquationSystem* fullSystem;

	if (context->IsMaster())
	{
		fullSystem = LinearEquationSystemFactory::Create(n);
	}
	else
	{
		fullSystem = new LinearEquationSystem(n);
	}

	int rowsPerProcess = n / context->NumberOfProcesses;
	LinearEquationSystem* partialSystem = new LinearEquationSystem(n, rowsPerProcess);

	communicator->Scatter(fullSystem->AugmentedMatrix[0], rowsPerProcess, partialSystem->AugmentedMatrix[0], rowType->Type);

	return partialSystem;
}