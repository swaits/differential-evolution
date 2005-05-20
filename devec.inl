#ifndef __devec_inl__
#define __devec_inl__


#if !(defined(__devec_h__))
#	 error "devec.inl should only be included by devec.h"
#endif


template <unsigned int DIM>
inline DE::Vector<DIM>::Vector()
{
	for (unsigned int i=0;i<DIM;i++)
	{
		tuple[i] = 0.0;
	}
	return;
}

template <unsigned int DIM>
inline DE::Vector<DIM>::~Vector()
{
	// empty
	return;
}

template <unsigned int DIM>
inline DE::Vector<DIM>::Vector(const Vector& other)
{
	*this = other;
	return;
}

template <unsigned int DIM>
inline DE::Vector<DIM>& DE::Vector<DIM>::operator= (const Vector& other)
{
	for (unsigned int i=0;i<DIM;i++)
	{
		tuple[i] = other.tuple[i];
	}
	return *this;
}

template <unsigned int DIM>
inline DE::Vector<DIM>& DE::Vector<DIM>::operator+= (const Vector& other)
{
	for (unsigned int i=0;i<DIM;i++)
	{
		tuple[i] += other.tuple[i];
	}
	return *this;
}

template <unsigned int DIM>
inline DE::Vector<DIM> DE::Vector<DIM>::operator+ (const Vector& other)
{
	Vector tmp(*this);
	tmp += other;
	return tmp;
}

template <unsigned int DIM>
inline DE::Vector<DIM>& DE::Vector<DIM>::operator-= (const Vector& other)
{
	for (unsigned int i=0;i<DIM;i++)
	{
		tuple[i] -= other.tuple[i];
	}
	return *this;
}

template <unsigned int DIM>
inline DE::Vector<DIM> DE::Vector<DIM>::operator- (const Vector& other)
{
	Vector tmp(*this);
	tmp -= other;
	return tmp;
}

template <unsigned int DIM>
inline DE::Vector<DIM>& DE::Vector<DIM>::operator*= (const double& multiplier)
{
	for (unsigned int i=0;i<DIM;i++)
	{
		tuple[i] *= multiplier;
	}
	return *this;
}

template <unsigned int DIM>
inline DE::Vector<DIM> DE::Vector<DIM>::operator* (const double& multiplier)
{
	Vector tmp(*this);
	tmp *= multiplier;
	return tmp;
}

template <unsigned int DIM>
inline DE::Vector<DIM>& DE::Vector<DIM>::operator/= (const double& divisor)
{
	for (unsigned int i=0;i<DIM;i++)
	{
		tuple[i] /= divisor;
	}
	return *this;
}

template <unsigned int DIM>
inline DE::Vector<DIM> DE::Vector<DIM>::operator/ (const double& divisor)
{
	Vector tmp(*this);
	tmp /= divisor;
	return tmp;
}

template <unsigned int DIM>
inline const double& DE::Vector<DIM>::operator[] (unsigned int i) const
{
	return tuple[i];
}

template <unsigned int DIM>
inline double& DE::Vector<DIM>::operator[] (unsigned int i)
{
	return tuple[i];
}

template <unsigned int DIM>
inline DE::Vector<DIM>::operator const double* () const
{
	return tuple;
}

template <unsigned int DIM>
inline DE::Vector<DIM>::operator double* ()
{
	return tuple;
}


#endif // __devec_inl__

