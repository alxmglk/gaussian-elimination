#include <stdio.h>
#include <windows.h>
#include <float.h>
#include "mpi.h"

#include "Stopwatch.h"
#include "LinearEquationSystemSolver.h"
#include "LinearEquationSystemSolutionChecker.h"
#include "MPIContext.h"
#include "MPICommunicator.h"
#include "LinearEquationSystemFactory.h"

Stopwatch stopwatch;
LinearEquationSystemSolutionChecker checker;

int n = 128;
const int nLimit = 128;
const int multiplier = 2;
const int repeatsNumber = 1;

int main(int argc, char* argv[])
{
	MPIContext context(&argc, &argv);
	MPICommunicator communicator(context, MPI_COMM_WORLD);
	LinearEquationSystemFactory factory(context, communicator);
	LinearEquationSystemSolver solver(context, communicator);

	for (; n <= nLimit; n *= multiplier)
	{
		float minTime = FLT_MAX;
		for (int attempt = 0; attempt < repeatsNumber; ++attempt)
		{
			LinearEquationSystem* partialSystem = factory.Create(n);
			NUMBER* solution = new NUMBER[n];

			stopwatch.Start();			
			solver.Solve(partialSystem, solution);
			stopwatch.Stop();

			float elapsedSeconds = stopwatch.GetElapsedSeconds();
			if (elapsedSeconds < minTime)
			{
				minTime = elapsedSeconds;
			}

			communicator.Barrier();
			
			//print(partialSystem);

			delete partialSystem;
			delete solution;
		}

		printf("N = %d, Elapsed seconds: %f\n", n, minTime);
	}
}