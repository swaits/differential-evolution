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

