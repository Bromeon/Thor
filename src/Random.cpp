/////////////////////////////////////////////////////////////////////////////////
//
// Thor C++ Library
// Copyright (c) 2011-2012 Jan Haller
// 
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
// 
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
// 
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would be
//    appreciated but is not required.
// 
// 2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
// 
// 3. This notice may not be removed or altered from any source distribution.
//
/////////////////////////////////////////////////////////////////////////////////

#include <Thor/Math/Random.hpp>

#include <SFML/Config.hpp>

#include AURORA_TR1_HEADER(random)
#include <ctime>
#include <cassert>
#include <climits>


namespace thor
{
namespace
{

#ifdef THOR_USE_TR1_RANDOMENGINE

	// Note: Don't use std::tr1::variate_generator, because it contains bugs.
	// If the distribution type is not unsigned long (like the engine's number type), a cast is performed,
	// truncating values and leading to completely wrong random numbers.

	// Use Mersenne Twister as default TR1 random engine
	typedef std::tr1::mt19937 Engine;

#else

	// Random generator engine (Multiply With Carry)
	// Many thanks to Volkard Henkel for the implementation.
	class Engine
	{
		public:
			// Type definition for usage inside TR1.Random
			typedef sf::Uint32 result_type;


		public:
			// Constructor
			explicit Engine(sf::Uint32 seedVal)
			{
				seed(seedVal);
			}
	
			// Return random number
			sf::Uint32 operator() ()
			{
				const sf::Uint64 a = 1967773755;

				x = a * (x & 0xffffffff) + (x >> 32);
				return static_cast<sf::Uint32>(x);
			}

			// set seed (compliant to TR1.Random)
			void seed(sf::Uint32 seedVal = 0)
			{
				x = seedVal + !seedVal;
			}

			// Return minimal value (compliant to TR1.Random)
			sf::Uint32 min() const
			{
				return 0;
			}

			// Return maximal value (compliant to TR1.Random)
			sf::Uint32 max() const
			{
				return 0xffffffff;
			}

		private:
			sf::Uint64 x;
	};

#endif // THOR_USE_TR1_RANDOMENGINE

	// Function initializing the engine and its seed at startup time
	Engine CreateInitialEngine()
	{
		return Engine( static_cast<unsigned long>(std::time(NULL)) );
	}

	// Pseudo random number generator engine
	Engine globalEngine = CreateInitialEngine();


	// Uniform real distribution for type float
	// Self-implemented, because std::tr1::uniform_real<float> is buggy on g++
	class UniformFloat
	{
		public:
			// Constructor
			UniformFloat(float min, float max)
			: mMin(min)
			, mMax(max)
			{
				assert(min <= max);
			}

			// Create uniform distribution from engine
			template <class Engine>
			float operator() (Engine& e)
			{
				// Scale to [0.f, 1.f]
				const float unit = static_cast<float>(e() - e.min()) / static_cast<float>(e.max() - e.min());

				// Scale to [mMin, mMax]
				return (mMax - mMin) * unit + mMin;
			}

		private:
			float mMin;
			float mMax;
	};

} // namespace

// ---------------------------------------------------------------------------------------------------------------------------


int random(int begin, int end)
{
	assert(begin <= end);
	std::tr1::uniform_int<int> distribution(begin, end);
	return distribution(globalEngine);
}

int randomDev(int middle, int deviation)
{
	assert(deviation >= 0);
	return random(middle-deviation, middle+deviation);
}

float random(float begin, float end)
{
	assert(begin <= end);
	UniformFloat distribution(begin, end);
	return distribution(globalEngine);
}

float randomDev(float middle, float deviation)
{
	assert(deviation >= 0.f);
	return random(middle-deviation, middle+deviation);
}

void setRandomSeed(unsigned long seed)
{
	globalEngine.seed(seed);
}

} // namespace thor
