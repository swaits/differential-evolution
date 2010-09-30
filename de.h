/*
The MIT License

Copyright (c) 2010, Stephen Waits <steve@waits.net>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/


/*
 * This is an implementation of Differential Evolution in C++. It's primarily
 * inline and has no external dependencies.
 *
 * DE is a simple, but useful stochastic optimization algorithm created by
 * Kenneth Price and Rainer Storn. This code is based, in part, on their 
 * original C version.
 *
 * See http://www.icsi.berkeley.edu/~storn/code.html for more information.
 *
 * The minimum you need to use this is to:
 * 1. inherit DE::Engine
 * 2. implement a concrete version of TestFitness in your derived class
 * 3. instance your class and run Solve
 *
 * See the examples/ directory for some semi-practical examples on how you
 * might make use of this code.
 *
 */

#ifndef __de_h__
#define __de_h__


#include "devec.h"
#include "derand.h"


namespace DE
{
	
	template <unsigned int DIM, unsigned int POPSIZE=(DIM*10)>
	class Engine
	{
	public:

		Engine();
		virtual ~Engine();

		const double* GetBest() const;

		void SetRange(double minimum, double maximum);
		void SetRange(unsigned int i, double minimum, double maximum);
		void SetRange(const Vector<DIM>& minimum, const Vector<DIM>& maximum);
		
		virtual double TestFitness(const double testcase[DIM], bool& stop) = 0;

		void Reset();
		bool Solve(unsigned int maxgenerations = 1000000000);
		bool RunOneGeneration();

	public:

		// helper class for accumulating error and calculating
		// root mean square error
		class ErrorAccumulator
		{
		public:
			ErrorAccumulator();
			virtual ~ErrorAccumulator();

			void Clear();
			void AddTestCase(double expected, double actual);
			double GetRMSE();

		private:

			void CalculateError();

			unsigned int num_tests;         // # of test cases
			double       sum_error_squared; // sum of error^2
			double       error_rmse;        // root mean square error (-1 == "dirty" flag)
		};

	private:

		// randomly select unique individuals
		void Select(
			unsigned int candidate,
			unsigned int *r1 = 0,
			unsigned int *r2 = 0,
			unsigned int *r3 = 0,
			unsigned int *r4 = 0,
			unsigned int *r5 = 0
		);

		// print a single individual to stdout
		void Dump(const double& fitness, Vector<DIM>& individual) const;

		// DE algorithm
		void MakeTrial_best1exp       (unsigned int candidate, Vector<DIM>& trial);
		void MakeTrial_rand1exp       (unsigned int candidate, Vector<DIM>& trial);
		void MakeTrial_randtobest1exp (unsigned int candidate, Vector<DIM>& trial);
		void MakeTrial_best2exp       (unsigned int candidate, Vector<DIM>& trial);
		void MakeTrial_rand2exp       (unsigned int candidate, Vector<DIM>& trial);
		void MakeTrial_best1bin       (unsigned int candidate, Vector<DIM>& trial);
		void MakeTrial_rand1bin       (unsigned int candidate, Vector<DIM>& trial);
		void MakeTrial_randtobest1bin (unsigned int candidate, Vector<DIM>& trial);
		void MakeTrial_best2bin       (unsigned int candidate, Vector<DIM>& trial);
		void MakeTrial_rand2bin       (unsigned int candidate, Vector<DIM>& trial);

		// constants, parameters
		static const double DEFAULTSCALE;
		static const double DEFAULTCROSSOVER;
		static const double DEFAULTRANGE;
		static const double BIGDOUBLE;
		double              scale;
		double              crossover;

		// entropy source
		Random prng;

		// population
		Vector<DIM> best;
		Vector<DIM> mean;
		Vector<DIM> variance;
		Vector<DIM> population[POPSIZE];
		double      fitness[POPSIZE];
		double      bestfitness;

		// algorithm state
		bool         success;
		unsigned int generation;

	};

};


#include "de.inl"


#endif // __de_h__

