#ifndef __devec_h__
#define __devec_h__

namespace DE
{
	
	
	template <unsigned int DIM>
	class Vector
	{
	public:

		// default constructor
		Vector();

		// destructor
		~Vector();

		// copy constructor
		Vector(const Vector& other);

		// assignment
		Vector& operator= (const Vector& other);

		// addition
		Vector& operator+= (const Vector& other);
		Vector  operator+  (const Vector& other);

		// subtraction
		Vector& operator-= (const Vector& other);
		Vector  operator-  (const Vector& other);

		// multiply by scalar
		Vector& operator*= (const double& multiplier);
		Vector  operator*  (const double& multiplier);

		// divide by scalar
		Vector& operator/= (const double& divisor);
		Vector  operator/  (const double& divisor);

		// element access
		const double& operator[] (unsigned int i) const;
		      double& operator[] (unsigned int i);

		// data access
		operator const double* () const;
		operator       double* ();

	private:

		// data lives here
		double tuple[DIM];
	};


};


#include "devec.inl"


#endif // __devec_h__

