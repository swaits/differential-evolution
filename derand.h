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



#ifndef __derand_h__
#define __derand_h__

namespace DE
{


	typedef unsigned int u32;
	typedef signed   int s32;
	
	
	class Random
	{
	public:

		Random();
	
		void Seed(u32 seed);
		
		u32 Rand();
		u32 operator()();
		
		float RandFloat();
		float RandFloat(float min, float max);
	
		double RandDouble();
		double RandDouble(double min, double max);

		double RandGaussian(double mean, double variance);
	
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

