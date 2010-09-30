/*
The MIT License

Copyright (c) 2010, Stephen Waits <steve@waits.net>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/



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

