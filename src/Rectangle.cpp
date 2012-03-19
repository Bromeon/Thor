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

#include <Thor/Geometry/Rectangle.hpp>
#include <Thor/Math/Random.hpp>


namespace thor
{

Rectangle::Rectangle(const sf::FloatRect& rect, float rotation)
: Zone(sf::Vector2f(rect.left + 0.5f*rect.width, rect.top + 0.5f*rect.height), rotation)
, mSize(rect.width, rect.height)
{
}

Rectangle::Rectangle(sf::Vector2f centerPosition, sf::Vector2f size, float rotation)
: Zone(centerPosition, rotation)
, mSize(size)
{
}

Rectangle::Rectangle(float centerX, float centerY, float width, float height, float rotation)
: Zone(sf::Vector2f(centerX, centerY), rotation)
, mSize(width, height)
{
}

void Rectangle::setSize(sf::Vector2f size)
{
	mSize = size;
}

sf::Vector2f Rectangle::getSize() const
{
	return mSize;
}

sf::Vector2f Rectangle::getRandomPoint() const
{
	return transformToGlobal(sf::Vector2f(
		randomDev(0.f, mSize.x/2.f),
		randomDev(0.f, mSize.y/2.f)));
}


Zone* Rectangle::clone() const
{
	return new Rectangle(*this);
}

} // namespace thor
