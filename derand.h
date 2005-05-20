#ifndef __derand_h__
#define __derand_h__

namespace DE
{


	typedef unsigned int u32;
	typedef signed   int s32;
	
	
	class Random
	{
	public:
	
		void Seed(u32 seed);
		
		u32 Rand();
		u32 operator()();
		
		float RandFloat();
		float RandFloat(float min, float max);
	
		double RandDouble();
		double RandDouble(double min, double max);
	
		u32 RandU32();
		u32 RandU32(u32 min, u32 max);
	
		s32 RandS32();
		s32 RandS32(s32 min, s32 max);
	
	

	private:
	
		static const u32 U32_MAX = (u32)0xffffffff;

		/**
		 * LCG multiplier by M. Lavaux and F. Janssens
		 */
		static const u32 MULTIPLIER = 1664525UL;
	
		/**
		 * LCG increment by M. Lavaux and F. Janssens
		 */
		static const u32 INCREMENT = 1013904223UL;
	
		/**
		 * LCG modulus set to 2^32, but not used due to MODULUS_ASSUMED
		 * above (since we're using a 32 bit word here)
		 *
		 * Set this to 0 if the modulus falls out automatically
		 * due to your data type (i.e. "addition modulo".  For
		 * instance, if you're using a 32 bit integer, and your
		 * modulus is 2^32, then there's no need to perform the
		 * modulo since   a == a % (2^32). (so the modulo is "free")
		 * 
		 */
		static const u32 MODULUS = 0xffffffff;
	
		/**
		 * LCG state
		 */
		u32 seed;
	};


};


#include "derand.inl"


#endif // __derand_h__

