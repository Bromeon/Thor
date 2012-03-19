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

#include <Thor/Geometry/Circle.hpp>
#include <Thor/Math/Random.hpp>
#include <Thor/Vectors/PolarVector.hpp>

#include <cassert>
#include <cmath>


namespace thor
{

Circle::Circle(sf::Vector2f centerPosition, float radius)
: Zone(centerPosition, 0.f) 
, mRadius(radius)
{
	assert(radius > 0.f);
}

void Circle::setRadius(float radius)
{
	assert(radius > 0.f);
	mRadius = radius;
}

float Circle::getRadius() const
{
	return mRadius;
}

sf::Vector2f Circle::getRandomPoint() const
{
	PolarVector2f randomPoint(mRadius * std::sqrt(random(0.f, 1.f)), random(0.f, 360.f));
	return transformToGlobal(randomPoint);
}

Zone* Circle::clone() const
{
	return new Circle(*this);
}

} // namespace thor
