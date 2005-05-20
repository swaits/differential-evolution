#ifndef __de_h__
#define __de_h__


#include "devec.h"
#include "derand.h"


namespace DE
{

	
	template <unsigned int DIM, unsigned int POP=(DIM*10)>
	class Engine
	{
	public:

		Engine();
		virtual ~Engine();

		void Reset();

		const double* GetBest() const;

		void SetRange(double minimum, double maximum);
		void SetRange(const Vector<DIM>& minimum, const Vector<DIM>& maximum);
		
		virtual double CalculateError(const double testcase[DIM], bool& stop) = 0;

		bool Solve(unsigned int maxgenerations = 1000000000);

	private:

		void Select(unsigned int candidate, unsigned int *r1 = 0, unsigned int *r2 = 0, unsigned int *r3 = 0, unsigned int *r4 = 0, unsigned int *r5 = 0);

		void Dump(const double& fitness, Vector<DIM>& individual) const;

		void MakeTrial_best1exp(unsigned int candidate, Vector<DIM>& trial);
		void MakeTrial_rand1exp(unsigned int candidate, Vector<DIM>& trial);
		void MakeTrial_randtobest1exp(unsigned int candidate, Vector<DIM>& trial);
		void MakeTrial_best2exp(unsigned int candidate, Vector<DIM>& trial);
		void MakeTrial_rand2exp(unsigned int candidate, Vector<DIM>& trial);
		void MakeTrial_best1bin(unsigned int candidate, Vector<DIM>& trial);
		void MakeTrial_rand1bin(unsigned int candidate, Vector<DIM>& trial);
		void MakeTrial_randtobest1bin(unsigned int candidate, Vector<DIM>& trial);
		void MakeTrial_best2bin(unsigned int candidate, Vector<DIM>& trial);
		void MakeTrial_rand2bin(unsigned int candidate, Vector<DIM>& trial);

	private:

		static const double DEFAULTSCALE;
		static const double DEFAULTCROSSOVER;
		static const double BIGDOUBLE;


	private:

		Random      prng;

		Vector<DIM> best;
		Vector<DIM> minimum;
		Vector<DIM> maximum;
		Vector<DIM> population[POP];
		double      fitness[POP];

		double      scale;
		double      crossover;
		double      bestfitness;

	};


};


#include "de.inl"


#endif // __de_h__

