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
		
		virtual double CalculateError(const double testcase[DIM], bool& stop) = 0;

		void Reset();
		bool Solve(unsigned int maxgenerations = 1000000000);
		bool RunOneGeneration();

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

