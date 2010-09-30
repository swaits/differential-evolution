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



#ifndef __derand_inl__
#define __derand_inl__

#include <ctime>

#if !(defined(__derand_h__))
#	 error "derand.inl should only be included by derand.h"
#endif

inline DE::Random::Random()
{
	Seed(static_cast<u32>(clock()));
}

inline void DE::Random::Seed(u32 seed)
{
	this->seed = seed;
}

inline DE::u32 DE::Random::Rand()
{
	seed = ( (seed * DE::Random::MULTIPLIER) + DE::Random::INCREMENT );
	seed %= DE::Random::MODULUS;
	
	return seed;
}

inline DE::u32 DE::Random::operator()()
{
	return this->Rand();
}

inline float DE::Random::RandFloat()
{
	return (float)Rand() / (float)DE::Random::U32_MAX;
}

inline float DE::Random::RandFloat(float min, float max)
{
	return ( RandFloat() * (max - min) + min );
}

inline double DE::Random::RandDouble()
{
	return (double)Rand() / (double)DE::Random::U32_MAX;
}

inline double DE::Random::RandDouble(double min, double max)
{
	return ( RandDouble() * (max - min) + min );
}

inline double DE::Random::RandGaussian(double mean, double variance)
{
	double sum = 0.0;
	for (int i=0;i<12;++i)
	{
		sum += RandDouble();
	}
	return mean + ( variance * ( sum - 6.0 ) );
}

inline DE::u32 DE::Random::RandU32()
{
	return Rand();
}

inline DE::u32 DE::Random::RandU32(DE::u32 min, DE::u32 max)
{
	return ( ( Rand() % (max - min) ) + min );
}

inline DE::s32 DE::Random::RandS32()
{
	return (DE::s32)Rand();
}

inline DE::s32 DE::Random::RandS32(DE::s32 min, DE::s32 max)
{
	return (DE::s32)( ( Rand() % (max - min) ) + min );
}

#endif // __derand_inl__

