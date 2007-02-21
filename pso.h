#ifndef __pso_h__
#define __pso_h__

namespace PSO
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


	};

};


#endif // __pso_h__

