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

#include <Thor/Math/Distributions.hpp>
#include <Thor/Math/Random.hpp>
#include <Thor/Vectors/VectorAlgebra2D.hpp>
#include <Thor/Vectors/PolarVector.hpp>

#include <cassert>


namespace thor
{
namespace
{

	// Functor for uniform distribution
	struct UniformFloat
	{
		UniformFloat(float begin, float end)
		: begin(begin)
		, end(end)
		{
		}

		float operator() ()
		{
			return random(begin, end);
		}

		float begin;
		float end;
	};

	// Functor for uniform distribution
	struct UniformTime
	{
		UniformTime(sf::Time begin, sf::Time end)
		: distribution(begin.asSeconds(), end.asSeconds())
		{
		}

		sf::Time operator() ()
		{
			return sf::seconds(distribution());
		}

		UniformFloat distribution;
	};

	// Functor for rect distribution
	struct Rect
	{
		Rect(sf::Vector2f center, sf::Vector2f halfSize)
		: center(center)
		, halfSize(halfSize)
		{
		}

		sf::Vector2f operator() ()
		{
			return sf::Vector2f(
				randomDev(center.x, halfSize.x),
				randomDev(center.y, halfSize.y));
		}

		sf::Vector2f center;
		sf::Vector2f halfSize;
	};

	// Functor for circle distribution
	struct Circle
	{
		Circle(sf::Vector2f center, float radius)
		: center(center)
		, radius(radius)
		{
		}

		sf::Vector2f operator() ()
		{
			sf::Vector2f radiusVector = PolarVector2f(radius * std::sqrt(random(0.f, 1.f)), random(0.f, 360.f));
			return center + radiusVector;
		}

		sf::Vector2f center;
		float radius;
	};

	// Functor for deflection
	struct Deflect
	{
		Deflect(sf::Vector2f direction, float maxRotation)
		: direction(direction)
		, maxRotation(maxRotation)
		{
		}

		sf::Vector2f operator() ()
		{
			return rotatedVector(direction, randomDev(0.f, maxRotation));
		}

		sf::Vector2f direction;
		float maxRotation;
	};

} // namespace

// ---------------------------------------------------------------------------------------------------------------------------


namespace Distributions
{
	
	thor::Distribution<float> uniform(float begin, float end)
	{
		assert(begin <= end);

		return UniformFloat(begin, end);
	}

	thor::Distribution<sf::Time> uniform(sf::Time begin, sf::Time end)
	{
		assert(begin <= end);

		return UniformTime(begin, end);
	}

	thor::Distribution<sf::Vector2f> rect(sf::Vector2f center, sf::Vector2f halfSize)
	{
		assert(halfSize.x >= 0.f && halfSize.y >= 0.f);

		return Rect(center, halfSize);
	}

	thor::Distribution<sf::Vector2f> circle(sf::Vector2f center, float radius)
	{
		assert(radius >= 0.f);

		return Circle(center, radius);
	}

	thor::Distribution<sf::Vector2f> deflect(sf::Vector2f direction, float maxRotation)
	{
		return Deflect(direction, maxRotation);
	}

} // namespace Distributions
} // namespace thor
