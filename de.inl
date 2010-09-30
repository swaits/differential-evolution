#ifndef __de_inl__
#define __de_inl__


#include <cstdio>
#include <cmath>


#if !(defined(__de_h__))
#	 error "de.inl should only be included by de.h"
#endif


template <unsigned int DIM, unsigned int POPSIZE>
const double DE::Engine<DIM,POPSIZE>::DEFAULTSCALE = 0.85;

template <unsigned int DIM, unsigned int POPSIZE>
const double DE::Engine<DIM,POPSIZE>::DEFAULTCROSSOVER = 1.0;

template <unsigned int DIM, unsigned int POPSIZE>
const double DE::Engine<DIM,POPSIZE>::DEFAULTRANGE= 0.1;

template <unsigned int DIM, unsigned int POPSIZE>
const double DE::Engine<DIM,POPSIZE>::BIGDOUBLE = 1.79e308; // close to max double

template <unsigned int DIM, unsigned int POPSIZE>
inline DE::Engine<DIM,POPSIZE>::Engine()
{
	// set defaults
	scale     = DEFAULTSCALE;
	crossover = DEFAULTCROSSOVER;
	for ( unsigned int i=0;i<DIM;++i )
	{
		this->mean[i]     = 0.0;
		this->variance[i] = DEFAULTRANGE;
	}

	// create an initial population
	Reset();

	return;
}

template <unsigned int DIM, unsigned int POPSIZE>
inline DE::Engine<DIM,POPSIZE>::~Engine()
{
	return;
}

template <unsigned int DIM, unsigned int POPSIZE>
inline void DE::Engine<DIM,POPSIZE>::Reset()
{
	// reset our best
	bestfitness = BIGDOUBLE;
	for ( unsigned int i=0;i<DIM;i++ )
	{
		best[i] = 0.0;
	}

	// reset all vectors
	for ( unsigned int i=0;i<POPSIZE;i++ )
	{
		// energy tied for worst
		fitness[i] = bestfitness;

		// make a new individual
		for ( unsigned int j=0;j<DIM;j++ )
		{
			population[i][j] = prng.RandGaussian(mean[j],variance[j]);
		}
	}

	// reset state
	generation = 0;
	success    = false;

	return;
}

template <unsigned int DIM, unsigned int POPSIZE>
inline void DE::Engine<DIM,POPSIZE>::SetRange(double minimum, double maximum)
{
	for (unsigned int i=0;i<DIM;i++)
	{
		SetRange(i,minimum,maximum);
	}

	return;
}

template <unsigned int DIM, unsigned int POPSIZE>
inline void DE::Engine<DIM,POPSIZE>::SetRange(unsigned int i, double minimum, double maximum)
{
	if (i<DIM)
	{
		this->mean[i]     = (maximum+minimum)/2.0;
		this->variance[i] = (maximum-minimum)/2.0;
	}

	return;
}

template <unsigned int DIM, unsigned int POPSIZE>
inline void DE::Engine<DIM,POPSIZE>::SetRange(const Vector<DIM>& minimum, const Vector<DIM>& maximum)
{
	for (unsigned int i=0;i<DIM;i++)
	{
		SetRange(i,minimum[i],maximum[i]);
	}

	return;
}

template <unsigned int DIM, unsigned int POPSIZE>
inline const double* DE::Engine<DIM,POPSIZE>::GetBest() const
{
	return best;
}

template <unsigned int DIM, unsigned int POPSIZE>
inline void DE::Engine<DIM,POPSIZE>::Select(
	unsigned int candidate,
	unsigned int *r1,
	unsigned int *r2,
	unsigned int *r3,
	unsigned int *r4,
	unsigned int *r5
)
{
	if ( r1 )
	{
		do
		{
			*r1 = prng.RandU32(0,POPSIZE-1);
		}
		while ( *r1 == candidate );
	}

	if ( r2 )
	{
		do
		{
			*r2 = prng.RandU32(0,POPSIZE-1);
		}
		while ( (*r2 == candidate) || (*r2 == *r1) );
	}

	if ( r3 )
	{
		do
		{
			*r3 = prng.RandU32(0,POPSIZE-1);
		}
		while ( (*r3 == candidate) || (*r3 == *r2) || (*r3 == *r1) );
	}

	if ( r4 )
	{
		do
		{
			*r4 = prng.RandU32(0,POPSIZE-1);
		}
		while ( (*r4 == candidate) || (*r4 == *r3) || (*r4 == *r2) || (*r4 == *r1) );
	}

	if ( r5 )
	{
		do
		{
			*r5 = prng.RandU32(0,POPSIZE-1);
		}
		while ( (*r5 == candidate) || (*r5 == *r4) || (*r5 == *r3) || (*r5 == *r2) || (*r5 == *r1) );
	}

	return;
}

template <unsigned int DIM, unsigned int POPSIZE>
inline bool DE::Engine<DIM,POPSIZE>::Solve(unsigned int maxgenerations)
{
	// spawn a new generation
	Reset();

	// run generations until max or success
	while(!success && generation<maxgenerations)
	{
		success = RunOneGeneration();
	}

	// output
	printf("====== RUN COMPLETE       =============================================\n\n");
	Dump(bestfitness,best);

	// finished
	return success;
}

template <unsigned int DIM, unsigned int POPSIZE>
bool DE::Engine<DIM,POPSIZE>::RunOneGeneration()
{
	printf("====== GENERATION %07d =============================================\n\n",++generation);

	// attempt to improve each individual in the population
	for (int candidate=0;candidate<POPSIZE && !success;candidate++)
	{
		// create a new trial individual to test
		Vector<DIM> trial;
		MakeTrial_randtobest1exp(candidate,trial);

		// determine fitness of trial individual
		double trialfitness = TestFitness(trial,success);

		// see if the trial improved current candidate in population
		if ( trialfitness < fitness[candidate] )
		{
			// it did improve, save it
			fitness[candidate]    = trialfitness;
			population[candidate] = trial;

			// see if it also improved upon our current best
			if ( trialfitness < bestfitness )
			{
				// it's better than the best, save it
				bestfitness = trialfitness;
				best        = trial;

				// output new best
				Dump(bestfitness,best);
			}
		}
	}

	return success;
}

template <unsigned int DIM, unsigned int POPSIZE>
inline void DE::Engine<DIM,POPSIZE>::Dump(const double& fitness, DE::Vector<DIM>& individual) const
{
	printf(" fitness = %24.16f\n",fitness);
	for (unsigned int i=0;i<DIM;i++)
	{
		printf("   [%03d] = %24.16e\n",i,individual[i]);
	}
	printf("\n");
}

template <unsigned int DIM, unsigned int POPSIZE>
inline void DE::Engine<DIM,POPSIZE>::MakeTrial_best1exp(unsigned int candidate, DE::Vector<DIM>& trial)
{
	unsigned int r1, r2;
	unsigned int n;

	Select(candidate, &r1, &r2);
	n = prng.RandU32(0,DIM-1);

	trial = population[candidate];
	for (unsigned int i=0; (prng.RandDouble(0.0,1.0) < crossover) && (i < DIM); i++) 
	{
		trial[n] = best[n] + scale * (population[r1][n] - population[r2][n]);
		n        = (n + 1) % DIM;
	}

	return;
}

template <unsigned int DIM, unsigned int POPSIZE>
inline void DE::Engine<DIM,POPSIZE>::MakeTrial_rand1exp(unsigned int candidate, DE::Vector<DIM>& trial)
{
	unsigned int r1, r2, r3;
	unsigned int n;

	Select(candidate, &r1, &r2, &r3);
	n = prng.RandU32(0,DIM-1);

	trial = population[candidate];
	for (unsigned int i=0; (prng.RandDouble(0.0,1.0) <= crossover) && (i < DIM); i++) 
	{
		trial[n] = population[r1][n] + scale * (population[r2][n] - population[r3][n]);
		n        = (n + 1) % DIM;
	}

	return;
}

template <unsigned int DIM, unsigned int POPSIZE>
inline void DE::Engine<DIM,POPSIZE>::MakeTrial_randtobest1exp(unsigned int candidate, DE::Vector<DIM>& trial)
{
	unsigned int r1, r2;
	unsigned int n;

	Select(candidate, &r1, &r2);
	n = prng.RandU32(0,DIM-1);

	trial = population[candidate];
	for (unsigned int i=0; (prng.RandDouble(0.0,1.0) <= crossover) && (i < DIM); i++) 
	{
		trial[n] += scale * (best[n] - trial[n]) + scale * (population[r1][n] - population[r2][n]);
		n         = (n + 1) % DIM;
	}

	return;
}

template <unsigned int DIM, unsigned int POPSIZE>
inline void DE::Engine<DIM,POPSIZE>::MakeTrial_best2exp(unsigned int candidate, DE::Vector<DIM>& trial)
{
	unsigned int r1, r2, r3, r4;
	unsigned int n;

	Select(candidate, &r1, &r2, &r3, &r4);
	n = prng.RandU32(0,DIM-1);

	trial = population[candidate];
	for (unsigned int i=0; (prng.RandDouble(0.0,1.0) <= crossover) && (i < DIM); i++) 
	{
		trial[n] = best[n] + scale * (population[r1][n] + population[r2][n] - population[r3][n] - population[r4][n]);
		n        = (n + 1) % DIM;
	}

	return;
}

template <unsigned int DIM, unsigned int POPSIZE>
inline void DE::Engine<DIM,POPSIZE>::MakeTrial_rand2exp(unsigned int candidate, DE::Vector<DIM>& trial)
{
	unsigned int r1, r2, r3, r4, r5;
	unsigned int n;

	Select(candidate, &r1, &r2, &r3 ,&r4, &r5);
	n = prng.RandU32(0,DIM-1);

	trial = population[candidate];
	for (unsigned int i=0; (prng.RandDouble(0.0,1.0) <= crossover) && (i < DIM); i++) 
	{
		trial[n] = population[r1][n] + scale * (population[r2][n] + population[r3][n] - population[r4][n] - population[r5][n]);
		n        = (n + 1) % DIM;
	}

	return;
}

template <unsigned int DIM, unsigned int POPSIZE>
inline void DE::Engine<DIM,POPSIZE>::MakeTrial_best1bin(unsigned int candidate, DE::Vector<DIM>& trial)
{
	unsigned int r1, r2;
	unsigned int n;

	Select(candidate, &r1, &r2);
	n = prng.RandU32(0,DIM-1);

	trial = population[candidate];
	for (unsigned int i=0; i < DIM; i++) 
	{
		if ( (prng.RandDouble(0.0,1.0) <= crossover) || (i == (DIM - 1)) )
		{
			trial[n] = best[n] + scale * (population[r1][n] - population[r2][n]);
		}
		n = (n + 1) % DIM;
	}

	return;
}

template <unsigned int DIM, unsigned int POPSIZE>
inline void DE::Engine<DIM,POPSIZE>::MakeTrial_rand1bin(unsigned int candidate, DE::Vector<DIM>& trial)
{
	unsigned int r1, r2, r3;
	unsigned int n;

	Select(candidate, &r1, &r2, &r3);
	n = prng.RandU32(0,DIM-1);

	trial = population[candidate];
	for (unsigned int i=0; i < DIM; i++) 
	{
		if ( (prng.RandDouble(0.0,1.0) <= crossover) || (i == (DIM - 1)) )
		{
			trial[n] = population[r1][n] + scale * (population[r2][n] - population[r3][n]);
		}
		n = (n + 1) % DIM;
	}

	return;
}

template <unsigned int DIM, unsigned int POPSIZE>
inline void DE::Engine<DIM,POPSIZE>::MakeTrial_randtobest1bin(unsigned int candidate, DE::Vector<DIM>& trial)
{
	unsigned int r1, r2;
	unsigned int n;

	Select(candidate, &r1, &r2);
	n = prng.RandU32(0,DIM-1);

	trial = population[candidate];
	for (unsigned int i=0; i < DIM; i++) 
	{
		if ( (prng.RandDouble(0.0,1.0) <= crossover) || (i == (DIM - 1)) )
		{
			trial[n] += scale * (best[n] - trial[n]) + scale * (population[r1][n] - population[r2][n]);
		}
		n = (n + 1) % DIM;
	}

	return;
}

template <unsigned int DIM, unsigned int POPSIZE>
inline void DE::Engine<DIM,POPSIZE>::MakeTrial_best2bin(unsigned int candidate, DE::Vector<DIM>& trial)
{
	unsigned int r1, r2, r3, r4;
	unsigned int n;

	Select(candidate, &r1, &r2, &r3, &r4);
	n = prng.RandU32(0,DIM-1);

	trial = population[candidate];
	for (unsigned int i=0; i < DIM; i++) 
	{
		if ( (prng.RandDouble(0.0,1.0) <= crossover) || (i == (DIM - 1)) )
		{
			trial[n] = best[n] + scale * (population[r1][n] + population[r2][n] - population[r3][n] - population[r4][n]);
		}
		n = (n + 1) % DIM;
	}

	return;
}

template <unsigned int DIM, unsigned int POPSIZE>
inline void DE::Engine<DIM,POPSIZE>::MakeTrial_rand2bin(unsigned int candidate, DE::Vector<DIM>& trial)
{
	unsigned int r1, r2, r3, r4, r5;
	unsigned int n;

	Select(candidate, &r1, &r2, &r3, &r4, &r5);
	n = prng.RandU32(0,DIM-1);

	trial = population[candidate];
	for (unsigned int i=0; i < DIM; i++) 
	{
		if ( (prng.RandDouble(0.0,1.0) <= crossover) || (i == (DIM - 1)) )
		{
			trial[n] = population[r1][n] + scale * (population[r2][n] + population[r3][n] - population[r4][n] - population[r5][n]);
		}
		n = (n + 1) % DIM;
	}

	return;
}




template <unsigned int DIM, unsigned int POPSIZE>
inline DE::Engine<DIM,POPSIZE>::ErrorAccumulator::ErrorAccumulator()
{
	Clear();
}

template <unsigned int DIM, unsigned int POPSIZE>
inline DE::Engine<DIM,POPSIZE>::ErrorAccumulator::~ErrorAccumulator()
{
	// empty
}

template <unsigned int DIM, unsigned int POPSIZE>
inline void DE::Engine<DIM,POPSIZE>::ErrorAccumulator::Clear()
{
	num_tests         = 0;
	sum_error_squared = 0.0;
	error_rmse        = 0.0; // initial error == zero
}

template <unsigned int DIM, unsigned int POPSIZE>
inline void DE::Engine<DIM,POPSIZE>::ErrorAccumulator::AddTestCase(double expected, double actual)
{
	double e           = expected - actual; // compute error
	sum_error_squared += e * e;             // add to sum of error^2
	num_tests         += 1;                 // increment total test count
	error_rmse         = -1.0;              // mark as dirty
}

template <unsigned int DIM, unsigned int POPSIZE>
inline double DE::Engine<DIM,POPSIZE>::ErrorAccumulator::GetRMSE()
{
	if ( error_rmse < 0.0 ) // recalculate if set to "dirty"
		CalculateError();

	return error_rmse;
}

template <unsigned int DIM, unsigned int POPSIZE>
inline void DE::Engine<DIM,POPSIZE>::ErrorAccumulator::CalculateError()
{
	// convert squared error to root mean squared error
	error_rmse = ::sqrt(sum_error_squared / (double)num_tests);
}



#endif // __de_inl__

