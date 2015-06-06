#include "LinearEquationSystemFactory.h"

LinearEquationSystemFactory::LinearEquationSystemFactory(MPIContext* mpiContext, MPICommunicator* mpiCommunicator)
{
	context = mpiContext;
	communicator = mpiCommunicator;
}

LinearEquationSystem* LinearEquationSystemFactory::Create(int n)
{
	int rowsPerProcess = n / context->NumberOfProcesses;
	LinearEquationSystem* system = new LinearEquationSystem(n);;
	LinearEquationSystem* partialSystem = new LinearEquationSystem(n, rowsPerProcess);	

	if (context->IsMaster())
	{
		Fill(system);
	}

	communicator->Scatter(system->AugmentedMatrix[0], rowsPerProcess, partialSystem->AugmentedMatrix[0], partialSystem->RowType->Type);

	delete system;

	return partialSystem;
}

void LinearEquationSystemFactory::Fill(LinearEquationSystem* system)
{
	int maxValue = 42;
	NUMBER divider = 3.33f;
	NUMBER** matrix = system->AugmentedMatrix;

	srand(time(nullptr));

	for (int row = 0; row < system->RowsCount; ++row)
	{
		NUMBER freeTerm = 0;

		int column = 0;
		for (; column < system->ColumnsCount - 1; ++column)
		{
			NUMBER value = (rand() % maxValue - (maxValue / 2)) / divider;

			matrix[row][column] = value;
			freeTerm += value * column;
		}

		matrix[row][column] = freeTerm;
	}
}