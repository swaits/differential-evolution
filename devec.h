#ifndef __devec_h__
#define __devec_h__

namespace DE
{
	
	
	/**
	 * A basic vector class.  Used by the DE::Engine to store genotypes.
	 */
	template <unsigned int DIM>
	class Vector
	{
	public:

		/**
		 * Default constructor
		 */
		Vector();

		/**
		 * Destructor
		 */
		~Vector();

		/**
		 * Copy constructor
		 * 
		 * @param other  vector to copy
		 */
		Vector(const Vector& other);

		/**
		 * Copy assignment
		 * 
		 * @param other  vector to copy
		 * 
		 * @return self reference
		 */
		Vector& operator= (const Vector& other);



		/**
		 * Addition update operator
		 * 
		 * @param other  vector to add
		 * 
		 * @return self reference (after addition)
		 */
		Vector& operator+= (const Vector& other);

		/**
		 * Addition operator
		 * 
		 * @param other  vector to add
		 * 
		 * @return resultant vector
		 */
		Vector operator+  (const Vector& other);


		/**
		 * Subtraction update operator
		 * 
		 * @param other  vector to subtract
		 * 
		 * @return self reference (after subtraction)
		 */
		Vector& operator-= (const Vector& other);

		/**
		 * Subtraction operator
		 * 
		 * @param other  vector to subtract
		 * 
		 * @return resultant vector
		 */
		Vector operator-  (const Vector& other);


		/**
		 * Multiplication update by scalar operator
		 * 
		 * @param multiplier scalar to scale by
		 * 
		 * @return self reference (after multiplication)
		 */
		Vector& operator*= (const double& multiplier);

		/**
		 * Multiplication by scalar operator
		 * 
		 * @param multiplier scalar to scale by
		 * 
		 * @return resultant vector
		 */
		Vector  operator*  (const double& multiplier);


		/**
		 * Division update by a scalar operator
		 * 
		 * @param divisor scalar to divide by
		 * 
		 * @return self reference (after division)
		 */
		Vector& operator/= (const double& divisor);

		/**
		 * Division by a scalar operator
		 * 
		 * @param divisor scalar to divide by
		 * 
		 * @return resultant vector
		 */
		Vector  operator/  (const double& divisor);



		/**
		 * Element access operator (const version)
		 * 
		 * @param i      index of element to access
		 * 
		 * @return const reference to element i
		 */
		const double& operator[] (unsigned int i) const;

		/**
		 * Element access operator
		 * 
		 * @param i      index of element to access
		 * 
		 * @return const reference to element i
		 */
		double& operator[] (unsigned int i);



		/**
		 * Access the vector data pointer (const version)
		 * 
		 * @return const pointer to vector data, array of doubles
		 */
		operator const double* () const;

		/**
		 * Access the vector data pointer
		 * 
		 * @return pointer to vector data, array of doubles
		 */
		operator double* ();



	private:

		/**
		 * Vector data array
		 */
		double tuple[DIM];

	};


};


#include "devec.inl"


#endif // __devec_h__

