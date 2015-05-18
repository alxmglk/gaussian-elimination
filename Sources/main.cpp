#include <stdio.h>
#include <math.h>
#include <windows.h>
#include <intrin.h>
#include <float.h>

#include "Stopwatch.h"
#include "SIMDExtensionsChecker.h"
#include "LinearEquationSystemFactory.h"
#include "LinearEquationSystemSolver.h"
#include "LinearEquationSystemSolutionChecker.h"

Stopwatch stopwatch;
SIMDExtensionsChecker simdExtensionsChecker;
LinearEquationSystemFactory factory;
LinearEquationSystemSolver solver;
LinearEquationSystemSolutionChecker checker;

int n = 32;
const int nLimit = 2048;
const int multiplier = 2;
const int repeatsNumber = 5;

int main()
{
	simdExtensionsChecker.PrintSupportedExtensions();

	for (; n <= nLimit; n *= multiplier)
	{
		float minTime = FLT_MAX;
		for (int i = 0; i < repeatsNumber; ++i)
		{
			LinearEquationSystem* s = factory.Create(n);
			NUMBER* solution = new NUMBER[n];
			 
			stopwatch.Start();
			solver.Solve(s, solution);
			stopwatch.Stop();

			float elapsedSeconds = stopwatch.GetElapsedSeconds();
			if (elapsedSeconds < minTime)
			{
				minTime = elapsedSeconds;
			}

			/*bool correct = checker.IsCorrectSolution(s, solution);
			printf("Solved: %s\n", correct ? "true" : "false");*/

			delete s;
			delete[] solution;
		}

		printf("N = %d, Elapsed seconds: %f\n", n, minTime);
	}

	system("pause");
}