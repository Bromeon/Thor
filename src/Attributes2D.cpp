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

#include <Thor/Geometry/Attributes2D.hpp>


namespace thor
{

Positionable::Positionable(sf::Vector2f position)
: mPosition(position)
{
}

void Positionable::setPosition(sf::Vector2f position)
{
	setPosition(position.x, position.y);
}
	 
void Positionable::setPosition(float x, float y)
{
	mPosition.x = x;
	mPosition.y = y;
}
	 
void Positionable::move(sf::Vector2f offset)
{
	move(offset.x, offset.y);
}
	 
void Positionable::move(float offsetX, float offsetY)
{
	mPosition.x += offsetX;
	mPosition.y += offsetY;
}

sf::Vector2f Positionable::getPosition() const
{
	return mPosition;
}

// ---------------------------------------------------------------------------------------------------------------------------


Rotatable::Rotatable(float rotation)
: mRotation(rotation)
{
}

void Rotatable::setRotation(float rotation)
{
	mRotation = rotation;
}
	 
void Rotatable::rotate(float angle)
{
	mRotation += angle;
}
	 
float Rotatable::getRotation() const
{
	return mRotation;
}

// ---------------------------------------------------------------------------------------------------------------------------


Scalable::Scalable(sf::Vector2f scale)
: mScale(scale)
{
}

void Scalable::setScale(sf::Vector2f scale)
{
	setScale(scale.x, scale.y);
}

void Scalable::setScale(float scaleX, float scaleY)
{
	mScale.x = scaleX;
	mScale.y = scaleY;
}

void Scalable::scale(sf::Vector2f factor)
{
	scale(factor.x, factor.y);
}

void Scalable::scale(float factorX, float factorY)
{
	mScale.x *= factorX;
	mScale.y *= factorY;
}

sf::Vector2f Scalable::getScale() const
{
	return mScale;
}

} // namespace thor
