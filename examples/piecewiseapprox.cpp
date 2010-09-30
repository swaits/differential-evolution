#include <cstdio>
#include <cmath>

#include "de.h"

// the order of the polynomial (actually the number of coefficients, i.e. 1 = constant, 2 = line, etc.)
#define NUM_COEFFICIENTS 7

// the function we're searching for
double TargetFunction(double x)
{
	//return 1.0 / (1.0 + exp(-4.9*x));               // Ken Stanley's NEAT activation
	return (1.0 / (1.0 + exp(-4.9*x)) - 0.5) * 2.0; // " " (signed version)
	//return (1.0 / (1.0 + exp(-x)) - 0.5) * 2.0;     // signed sigmoid
	//return exp(-x*x);                               // gaussian
	//return tanh(x);                                 // tanh
	//return (2.0/(1.0+exp(-x))-1.0);                 // tanh approximation
	//return (x<0.0 ? -1.0 : 1.0) * sqrt(abs(x));     // signed root
	//return sin(x);                                  // sin
}


// a helper function used in calculating the polynomials
double CalculatePiecewise(double x, int n_coeffs, const double coeffs[])
{
	double abs_x = (x < 0.0 ? -x : x);
	const double* pcoeffs;
	if ( abs_x < coeffs[6] )
		pcoeffs = &coeffs[0];
	else
		pcoeffs = &coeffs[3];

	return (pcoeffs[0] + pcoeffs[1]*abs_x + pcoeffs[2]*abs_x*abs_x) * (x < 0.0 ? -1.0 : 1.0);
}


// our DE implementation
class PolySearch: public DE::Engine<NUM_COEFFICIENTS>
{
	public:
		// this method is called by the DE::Engine::Solve() to test a single DE individual
		double TestFitness(const double testcase[NUM_COEFFICIENTS], bool& stop)
		{
			ErrorAccumulator e;

			// accumulate squared error
			for ( double i=-2.5;i<=2.5;i+=0.01 )
			{
				// test
				double actual   = CalculatePiecewise(i,NUM_COEFFICIENTS,testcase);
				double expected = TargetFunction(i);

				// accumulate error
				e.AddTestCase(expected,actual);
			}

			// set stop flag if the error is really low
			if ( e.GetRMSE() < 0.0000001 )
			{
				stop = true;
			}

			// done
			return e.GetRMSE();
		}
};


// main program
int main()
{
	PolySearch de;

	// setup, adjust from default range
	//de.SetRange(-0.1,0.1);

	// reset, and run the DE
	de.Solve(1000);

	// done
	return 0;
}
