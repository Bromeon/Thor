/////////////////////////////////////////////////////////////////////////////////
//
// Thor C++ Library
// Copyright (c) 2011-2022 Jan Haller
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

#include <random>
#include <ctime>
#include <cassert>


namespace thor
{
namespace
{

#ifdef THOR_USE_STD_RANDOMENGINE

	// Use Mersenne Twister as default standard random engine
	typedef std::mt19937 Engine;

#else

	// Random generator engine (Multiply With Carry)
	// Many thanks to Volkard Henkel for the implementation.
	class Engine
	{
		public:
			// Type definition for usage inside Std.Random
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

			// set seed (compliant to Std.Random)
			void seed(sf::Uint32 seedVal = 0)
			{
				x = seedVal + !seedVal;
			}

			// Return minimal value (compliant to Std.Random)
			static constexpr sf::Uint32 min()
			{
				return 0;
			}

			// Return maximal value (compliant to Std.Random)
			static constexpr sf::Uint32 max()
			{
				return 0xffffffff;
			}

		private:
			sf::Uint64 x;
	};

#endif // THOR_USE_STD_RANDOMENGINE

	// Function initializing the engine and its seed at startup time
	Engine createInitialEngine()
	{
		return Engine( static_cast<unsigned long>(std::time(nullptr)) );
	}

	// Pseudo random number generator engine
	Engine globalEngine = createInitialEngine();

} // namespace

// ---------------------------------------------------------------------------------------------------------------------------


int random(int min, int max)
{
	assert(min <= max);
	std::uniform_int_distribution<int> distribution(min, max);
	return distribution(globalEngine);
}

unsigned int random(unsigned int min, unsigned int max)
{
	assert(min <= max);
	std::uniform_int_distribution<unsigned int> distribution(min, max);
	return distribution(globalEngine);
}

float random(float min, float max)
{
	assert(min <= max);
	std::uniform_real_distribution<float> distribution(min, max);
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
