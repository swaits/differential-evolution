#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <cassert>
//#include <vector>
//#include <algorithm>


const double PI = 3.141592653589793238462643383279502884197169399375105820974944592307816406286208998628034825342117;


#include <conio.h>

#include "de.h"


class PolyDEother: public DE::Engine<5>
{
public:

	PolyDEother()
	{
		SetRange(-20000000.0,200000000.0);
	}
	
	virtual double CalculateError(const double testcase[5], bool &stop)
	{
		double err = 0.0;
		int    n   = 0;

		for (double x=-10.0;x<=10.0;x+=0.025)
		{
			double diff = actual(x) - test(x,testcase);
			err += (diff * diff);
			n++;
		}

		err /= (double)n;

		if ( err < 0.00000000000000000001 )
		{
			stop = true;
		}

		return err;
	}

	static inline double actual(double x)
	{
		const double coeffs[] = { 1.0, 874.0, -451.0, 0.00000000001, -20000000000.0 };

		return test(x,coeffs);
	}

	static inline double test(double x, const double coeffs[5])
	{
		return coeffs[0] + coeffs[1]*x + coeffs[2]*x*x + coeffs[3]*x*x*x + coeffs[4]*x*x*x*x;
	}



};


#define  DIM  (10)

class PolyDE: public DE::Engine<DIM>
{
public:

	double actual_scale0[401*401];
	double actual_scale1[401*401];

	PolyDE()
	{
		SetRange(-0.2,0.2);

		int i=0;
		for (double t=0.0; t<=1.0; t+=0.0025)
		{
			for (double c=0.0; c<=1.0; c+=0.0025)
			{
				slerp_actual_t(t,c,actual_scale0[i],actual_scale1[i]);
				i++;
				assert(i<(402*402));
			}
		}

	}

	virtual double CalculateError(const double testcase[DIM], bool &stop)
	{
		double       sumerr = 0.0;
		unsigned int n      = 0;
		double err;

		int i = 0;
		for (double t=0.0; t<=1.0; t+=0.0025)
		{
			for (double c=0.0; c<=1.0; c+=0.0025)
			{
				double test0,test1;
				slerp_poly_t(t,c,testcase,test0,test1);
				
				err = actual_scale0[i] - test0;
				sumerr += err * err;
				err = actual_scale1[i] - test1;
				sumerr += err * err;

				i++;
				n++;
			}
		}

		sumerr /= (double)n;

		if ( sumerr < 0.0000000000000000000001 || _kbhit() )
		{
			stop = true;
		}

		return sumerr;
	}



	static inline void slerp_actual_t(const double& t, const double& costheta, double& scale0, double& scale1)
	{
		// calculate coefficients
		if ( costheta < 1.00000 )
		{
			double theta    = acos(costheta);
			double sintheta = sin(theta);
			       scale0   = sin((1.0 - t) * theta) / sintheta;
			       scale1   = sin(t * theta) / sintheta;
		}
		else
		{
			scale0 = 1.0 - t;
			scale1 = t;
		}
	}

	static inline void slerp_poly_t(const double& t, const double& c, const double coeffs[DIM], double& scale0, double& scale1)
	{
		scale0 = sum_poly2(t,c,&coeffs[0]);
		scale1 = sum_poly2(t,c,&coeffs[DIM>>1]);
	}



	static inline double sum_poly2(const double& x, const double& y, const double coeffs[5])
	{
		const double x2 = x*x;
		const double y2 = y*y;

		return coeffs[0] + coeffs[1]*x + coeffs[2]*x2 + coeffs[3]*y + coeffs[4]*y2;
	}
	
	static inline double sum_poly3(const double& x, const double& y, const double coeffs[7])
	{
		const double x2 = x*x;
		const double y2 = y*y;
		const double x3 = x2*x;
		const double y3 = y2*y;

		return coeffs[0] + coeffs[1]*x + coeffs[2]*x2 + coeffs[3]*x3 + coeffs[4]*y + coeffs[5]*y2 + coeffs[6]*y3;
	}
	
	static inline double sum_poly4(const double& x, const double& y, const double coeffs[9])
	{
		const double x2 = x*x;
		const double y2 = y*y;
		const double x3 = x2*x;
		const double y3 = y2*y;
		const double x4 = x3*x;
		const double y4 = y3*y;

		return coeffs[0] + coeffs[1]*x + coeffs[2]*x2 + coeffs[3]*x3 + coeffs[4]*x4 + coeffs[5]*y + coeffs[6]*y2 + coeffs[7]*y3 + coeffs[8]*y4;
	}
	
	static inline double bivariate_poly3(const double& x, const double& y, const double coeffs[10])
	{
		const double x2 = x*x;
		const double y2 = y*y;
		const double x3 = x2*x;
		const double y3 = y2*y;

		return x3*(coeffs[9]*y3+coeffs[7]*y2)+x2*(coeffs[8]*y3+coeffs[6]*y2+coeffs[4]*y)+x*(coeffs[5]*y2+coeffs[3]*y+coeffs[1])+coeffs[2]*y+coeffs[0];
	}

	static inline double bivariate_poly2(const double& x, const double& y, const double coeffs[7])
	{
		const double x2 = x*x;
		const double y2 = y*y;
		return x2*(coeffs[6]*y2 + coeffs[4]*y) + x*(coeffs[5]*y2 + coeffs[3]*y + coeffs[1]) + coeffs[2]*y + coeffs[0];
	}

	static inline double bivariate_poly1(const double& x, const double& y, const double coeffs[4])
	{
		return x*(coeffs[3]*y + coeffs[1]) + coeffs[2]*y + coeffs[0];
	}

};

int main()
{
	PolyDE* de = new PolyDE;

	de->Solve();

	delete de;

	return 0;
}










#if 0

class quat
{
public:

	double tuple[4];

	static const quat ZERO;
	static const quat IDENTITY;

	quat()
	{
		*this = quat::IDENTITY;
	}

	~quat()
	{
	}

	quat(const quat& other)
	{
		*this = other;
	}

	quat& operator=(const quat& other)
	{
		tuple[0] = other.tuple[0];
		tuple[1] = other.tuple[1];
		tuple[2] = other.tuple[2];
		tuple[3] = other.tuple[3];
		return *this;
	}

	quat(const double& x, const double& y, const double& z, const double& w)
	{
		tuple[0] = x;
		tuple[1] = y;
		tuple[2] = z;
		tuple[3] = w;
	}

	quat(const double axis[3], const double& angle)
	{
		double halfangle = angle / 2.0;
		double sinangle  = sin(halfangle);
	
		tuple[0] = axis[0] * sinangle;
		tuple[1] = axis[1] * sinangle;
		tuple[2] = axis[2] * sinangle;
		tuple[3] = cos(halfangle);
	}
	
	double dot(const quat& other) const
	{
		return (tuple[0]*other.tuple[0] + tuple[1]*other.tuple[1] + tuple[2]*other.tuple[2] + tuple[3]*other.tuple[3]);
	}

	quat& operator*= (double scalar)
	{
		tuple[0] *= scalar;
		tuple[1] *= scalar;
		tuple[2] *= scalar;
		tuple[3] *= scalar;
		return *this;
	}

	quat operator* (double scalar) const
	{
		return quat
		(
			tuple[0] * scalar,
			tuple[1] * scalar,
			tuple[2] * scalar,
			tuple[3] * scalar
		);
		return *this;
	}

	quat& operator/= (double scalar)
	{
		return (*this *= 1.0/scalar);
	}

	quat operator/ (double scalar) const
	{
		return (*this * 1.0/scalar);
	}

	double length_squared() const
	{
		return dot(*this);
	}

	double length() const
	{
		return sqrt(length_squared());
	}

	quat& normalize()
	{
		return (*this /= length());
	}

	quat& operator-()
	{
		tuple[0] = -tuple[0];
		tuple[1] = -tuple[1];
		tuple[2] = -tuple[2];
		tuple[3] = -tuple[3];
		return *this;
	}

	double distance(const quat& other)
	{
		quat tmp(tuple[0]-other.tuple[0],tuple[1]-other.tuple[1],tuple[2]-other.tuple[2],tuple[3]-other.tuple[3]);
		return tmp.length();
	}
};

void slerp_coeffs(double t, const quat& q0, const quat& q1, double& scale0, double& scale1)
{
	quat from(q0), to(q1);
	double costheta,sintheta,theta;

	// calculate cosine theta
	costheta = from.dot(to);

	// adjust signs if necessary
	if ( costheta < 0.0 )
	{
		costheta = -costheta;
		to       = -to;
	}

	// calculate coefficients
	if ( costheta < 1.00000 )
	{
		theta    = acos(costheta);
		sintheta = sin(theta);
		scale0   = sin((1.0 - t) * theta) / sintheta;
		scale1   = sin(t * theta) / sintheta;
	}
	else
	{
		scale0 = 1.0 - t;
		scale1 = t;
	}

	//printf("t=%f  s0=%0.10f  s1=%0.10f\n",t,scale0,scale1);
}

quat slerp(double t, const quat& q0, const quat& q1)
{
	double scale0,scale1;
	slerp_coeffs(t,q0,q1,scale0,scale1);

	// return result
	quat result
	(
		scale0 * q0.tuple[0] + scale1 * q1.tuple[0],
		scale0 * q0.tuple[1] + scale1 * q1.tuple[1],
		scale0 * q0.tuple[2] + scale1 * q1.tuple[2],
		scale0 * q0.tuple[3] + scale1 * q1.tuple[3]
	);

	result.normalize();

	return result;
}

quat lerp(double t, const quat& q0, const quat& q1)
{
	double scale0 = 1.0 - t;
	double scale1 = t;

	return quat
	(
		scale0 * q0.tuple[0] + scale1 * q1.tuple[0],
		scale0 * q0.tuple[1] + scale1 * q1.tuple[1],
		scale0 * q0.tuple[2] + scale1 * q1.tuple[2],
		scale0 * q0.tuple[3] + scale1 * q1.tuple[3]
	);
}

quat slerp_via_lerp_and_normalize(double t, const quat& q0, const quat& q1)
{
	double scale0,scale1;
	slerp_coeffs(t,q0,q1,scale0,scale1);

	double new_t = scale1 / (scale0+scale1);

	quat result = lerp(new_t,q0,q1);

	result.normalize();

	return result;
}

const quat quat::ZERO     = quat(0.0,0.0,0.0,0.0);
const quat quat::IDENTITY = quat(0.0,0.0,0.0,1.0);

double vec_length(double vec[3])
{
	return sqrt(vec[0]*vec[0] + vec[1]*vec[1] + vec[2]*vec[2]);
}

double random_double(double low, double high)
{
	double prn = (double)rand() / (double)RAND_MAX;

	return (prn * (high - low)) + low;
}

quat random_quat()
{
	double axis[3];
	double angle;

	// random vector elements
	axis[0] = random_double(-1.0,1.0);
	axis[1] = random_double(-1.0,1.0);
	axis[2] = random_double(-1.0,1.0);
	
	// normalize vector
	double veclen = vec_length(axis);
	axis[0] /= veclen;
	axis[1] /= veclen;
	axis[2] /= veclen;

	// random angle
	angle = random_double(-PI*2.0,PI*2.0);

	// construct a quat
	return quat(axis,angle);
}



bool test()
{
	const double MAX_ERROR = 0.0000000000001;

	quat a = random_quat();
	quat b = random_quat();

	for (double t=0.0; t < 1.01; t+= 0.01)
	{
		quat c = slerp(t,a,b);
		quat d = slerp_via_lerp_and_normalize(t,a,b);

		double error = c.distance(d);
		if ( error > MAX_ERROR )
		{
			printf("actual error = %0.34f, max error = %0.34f\n",error,MAX_ERROR);
			return false;
		}
	}

	return true;
}

/*
quat slerp_approx(double t, const quat& q0, const quat& q1)
{
	quat from(q0),to(q1);

	// calculate cosine theta
	double costheta = from.dot(to);

	// adjust signs if necessary
	if ( costheta < 0.0 )
	{
		costheta = -costheta;
		to       = -to;
	}

	double new_t = poly[0] + poly[1]*t + poly[2]*t*t + poly[3] + poly[4]*costheta + poly[5]*costheta*costheta;

	quat result = lerp(new_t,q0,q1);
	result.normalize();

	return result;
}
*/


#endif
