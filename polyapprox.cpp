#include <cstdio>
#include <cmath>

#include "de.h"

// the order of the polynomial (actually the number of coefficients, i.e. 1 = constant, 2 = line, etc.)
#define NUM_COEFFICIENTS 8

// the function we're searching for
double TargetFunction(double x)
{
	return 1.0 / (1.0 + exp(-4.9*x));               // Ken Stanley's NEAT activation
	//return (1.0 / (1.0 + exp(-4.9*x)) - 0.5) * 2.0; // " " (signed version)
	//return (1.0 / (1.0 + exp(-x)) - 0.5) * 2.0;     // signed sigmoid
	//return exp(-x*x);                               // gaussian
	//return tanh(x);                                 // tanh
	//return (2.0/(1.0+exp(-x))-1.0);                 // tanh approximation
	//return (x<0.0 ? -1.0 : 1.0) * sqrt(abs(x));     // signed root
	//return sin(x);                                  // sin
}


// a helper function used in calculating the polynomials
double CalculatePolynomial(double x, int n_coeffs, const double coeffs[])
{
	double accum = 0.0;
	double xmul  = 1.0;
	for (int i = n_coeffs-1; i>=0; --i)
	{
		accum += xmul * coeffs[i];
		xmul *= x;
	}
	return accum;
}


// our DE implementation
class PolySearch: public DE::Engine<NUM_COEFFICIENTS>
{
	public:
		// this method is called by the DE::Engine::Solve() to test a single DE individual
		double CalculateError(const double testcase[NUM_COEFFICIENTS], bool& stop)
		{
			double       error      = 0.0;
			unsigned int test_count = 0;

			// accumulate squared error
			for ( double i=-1.25;i<=1.25;i+=0.001 )
			{
				// test
				double test   = CalculatePolynomial(i,NUM_COEFFICIENTS,testcase);
				double actual = TargetFunction(i);

				// accumulate error^2
				double e = test - actual;
				error += e * e;

				// inc test count (divisor to get from SE to MSE)
				test_count++;
			}

			// convert SE to RMSE
			error = sqrt(error / (double)test_count);

			// set stop flag if the error is really low
			if ( error < 0.0000001 )
			{
				stop = true;
			}

			// done
			return error;
		}
};


// main program
int main()
{
	PolySearch de;

	// setup, adjust from default range
	de.SetRange(-0.1,0.1);

	// create new population with adjusted range
	de.Reset();

	// run the DE
	de.Solve(1500);

	// output winner in polynomial form
	printf("Best Polynomial Found\n=====================\ny =");
	for (int i=0;i<NUM_COEFFICIENTS-2;++i)
	{
		printf(" %0.16f*x^%d +",de.GetBest()[i],NUM_COEFFICIENTS-1-i);
	}
	printf(" %0.16f*x + %0.16f\n",de.GetBest()[NUM_COEFFICIENTS-2],de.GetBest()[NUM_COEFFICIENTS-1]);

	// done
	return 0;
}
