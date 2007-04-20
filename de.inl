#ifndef __de_inl__
#define __de_inl__


#include <cstdio>


#if !(defined(__de_h__))
#	 error "de.inl should only be included by de.h"
#endif


template <unsigned int DIM, unsigned int POP>
const double DE::Engine<DIM,POP>::DEFAULTSCALE = 0.85;

template <unsigned int DIM, unsigned int POP>
const double DE::Engine<DIM,POP>::DEFAULTCROSSOVER = 1.0;

template <unsigned int DIM, unsigned int POP>
const double DE::Engine<DIM,POP>::DEFAULTRANGE= 100.0;

template <unsigned int DIM, unsigned int POP>
const double DE::Engine<DIM,POP>::BIGDOUBLE = 1.79e308; // close to max double

template <unsigned int DIM, unsigned int POP>
inline DE::Engine<DIM,POP>::Engine()
{
	scale     = DEFAULTSCALE;
	crossover = DEFAULTCROSSOVER;

	for ( unsigned int i=0;i<DIM;i++ )
	{
		this->minimum[i] = -DEFAULTRANGE;
		this->maximum[i] = -(this->minimum[i]);
	}

	Reset();

	return;
}

template <unsigned int DIM, unsigned int POP>
inline DE::Engine<DIM,POP>::~Engine()
{
	return;
}

template <unsigned int DIM, unsigned int POP>
inline void DE::Engine<DIM,POP>::Reset()
{
	// reset our best
	bestfitness = BIGDOUBLE;
	for ( unsigned int i=0;i<DIM;i++ )
	{
		best[i] = 0.0;
	}

	// reset all vectors
	for ( unsigned int i=0;i<POP;i++ )
	{
		// energy tied for worst
		fitness[i] = bestfitness;

		// make a new individual
		for ( unsigned int j=0;j<DIM;j++ )
		{
			population[i][j] = prng.RandDouble(minimum[j],maximum[j]);
		}
	}

	return;
}

template <unsigned int DIM, unsigned int POP>
inline void DE::Engine<DIM,POP>::SetRange(double minimum, double maximum)
{
	for (unsigned int i=0;i<DIM;i++)
	{
		this->minimum[i] = minimum;
		this->maximum[i] = maximum;
	}

	return;
}

template <unsigned int DIM, unsigned int POP>
inline void DE::Engine<DIM,POP>::SetRange(const Vector<DIM>& minimum, const Vector<DIM>& maximum)
{
	this->minimum = minimum;
	this->maximum = maximum;

	return;
}

template <unsigned int DIM, unsigned int POP>
inline const double* DE::Engine<DIM,POP>::GetBest() const
{
	return best;
}

template <unsigned int DIM, unsigned int POP>
inline void DE::Engine<DIM,POP>::Select(unsigned int candidate, unsigned int *r1, unsigned int *r2, unsigned int *r3, unsigned int *r4, unsigned int *r5)
{
	if ( r1 )
	{
		do
		{
			*r1 = prng.RandU32(0,POP-1);
		}
		while ( *r1 == candidate );
	}

	if ( r2 )
	{
		do
		{
			*r2 = prng.RandU32(0,POP-1);
		}
		while ( (*r2 == candidate) || (*r2 == *r1) );
	}

	if ( r3 )
	{
		do
		{
			*r3 = prng.RandU32(0,POP-1);
		}
		while ( (*r3 == candidate) || (*r3 == *r2) || (*r3 == *r1) );
	}

	if ( r4 )
	{
		do
		{
			*r4 = prng.RandU32(0,POP-1);
		}
		while ( (*r4 == candidate) || (*r4 == *r3) || (*r4 == *r2) || (*r4 == *r1) );
	}

	if ( r5 )
	{
		do
		{
			*r5 = prng.RandU32(0,POP-1);
		}
		while ( (*r5 == candidate) || (*r5 == *r4) || (*r5 == *r3) || (*r5 == *r2) || (*r5 == *r1) );
	}

	return;
}

// TODO: change this to run one generation at a time???
template <unsigned int DIM, unsigned int POP>
inline bool DE::Engine<DIM,POP>::Solve(unsigned int maxgenerations)
{
	bool success = false;

	Reset();

	for (unsigned int generation=0;generation<maxgenerations && !success;generation++)
	{
		printf("====== GENERATION %07d =============================================\n\n",generation);

		for (int candidate=0;candidate<POP && !success;candidate++)
		{
			Vector<DIM> trial;
			MakeTrial_randtobest1exp(candidate,trial);

			double trialfitness = CalculateError(trial,success);

			if ( trialfitness < fitness[candidate] )
			{
				fitness[candidate]    = trialfitness;
				population[candidate] = trial;

				if ( trialfitness < bestfitness )
				{
					bestfitness = trialfitness;
					best        = trial;
					
					Dump(bestfitness,best);
				}
			}
		}
	}

	printf("====== RUN COMPLETE       =============================================\n\n");
	Dump(bestfitness,best);

	return success;
}

template <unsigned int DIM, unsigned int POP>
inline void DE::Engine<DIM,POP>::Dump(const double& fitness, DE::Vector<DIM>& individual) const
{
	printf(" fitness = %24.16f\n",fitness);
	for (unsigned int i=0;i<DIM;i++)
	{
		printf("   [%03d] = %24.16e\n",i,individual[i]);
	}
	printf("\n");
}

template <unsigned int DIM, unsigned int POP>
inline void DE::Engine<DIM,POP>::MakeTrial_best1exp(unsigned int candidate, DE::Vector<DIM>& trial)
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

template <unsigned int DIM, unsigned int POP>
inline void DE::Engine<DIM,POP>::MakeTrial_rand1exp(unsigned int candidate, DE::Vector<DIM>& trial)
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

template <unsigned int DIM, unsigned int POP>
inline void DE::Engine<DIM,POP>::MakeTrial_randtobest1exp(unsigned int candidate, DE::Vector<DIM>& trial)
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

template <unsigned int DIM, unsigned int POP>
inline void DE::Engine<DIM,POP>::MakeTrial_best2exp(unsigned int candidate, DE::Vector<DIM>& trial)
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

template <unsigned int DIM, unsigned int POP>
inline void DE::Engine<DIM,POP>::MakeTrial_rand2exp(unsigned int candidate, DE::Vector<DIM>& trial)
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

template <unsigned int DIM, unsigned int POP>
inline void DE::Engine<DIM,POP>::MakeTrial_best1bin(unsigned int candidate, DE::Vector<DIM>& trial)
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

template <unsigned int DIM, unsigned int POP>
inline void DE::Engine<DIM,POP>::MakeTrial_rand1bin(unsigned int candidate, DE::Vector<DIM>& trial)
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

template <unsigned int DIM, unsigned int POP>
inline void DE::Engine<DIM,POP>::MakeTrial_randtobest1bin(unsigned int candidate, DE::Vector<DIM>& trial)
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

template <unsigned int DIM, unsigned int POP>
inline void DE::Engine<DIM,POP>::MakeTrial_best2bin(unsigned int candidate, DE::Vector<DIM>& trial)
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

template <unsigned int DIM, unsigned int POP>
inline void DE::Engine<DIM,POP>::MakeTrial_rand2bin(unsigned int candidate, DE::Vector<DIM>& trial)
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

#endif // __de_inl__

