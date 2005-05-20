#include <cstdio>

#include "de.h"

// just a helper function used in calculating the polynomails
double Polynomial(double x, double a, double b, double c, double d)
{
	return a*(x*x*x) + b*(x*x) + c*x + d;
}

// the function we're searching for
double TargetFunction(double input)
{
	return Polynomial(input,17.0,321.0,-2.0,324.0);
}

// our DE implementation
class PolySearch: public DE::Engine<4>
{
	public:
		double CalculateError(const double testcase[4], bool& stop)
		{
			double error = 0.0;
			for ( int i=-100;i<=100;i++ )
			{
				double test   = Polynomial((double)i,testcase[0],testcase[1],testcase[2],testcase[3]);
				double actual = TargetFunction((double)i);
				error += (test-actual)*(test-actual);
			}
			if ( error < 0.00000000000001 )
			{
				stop = true;
			}
			return error;
		}
};

// main program
int main()
{
	PolySearch de;
	de.Solve();
	return 0;
}
