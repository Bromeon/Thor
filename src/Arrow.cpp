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

#include <Thor/Shapes/Arrow.hpp>
#include <Thor/Shapes/Shapes.hpp>
#include <Thor/Vectors/VectorAlgebra2D.hpp>

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/CircleShape.hpp>

#include <cassert>


namespace thor
{

float Arrow::zeroVectorTolerance = 0.5f;

Arrow::Arrow(sf::Vector2f position, sf::Vector2f direction, const sf::Color& color, float thickness)
: sf::Drawable()
, sf::Transformable()
, mThickness(thickness)
, mLength(length(direction))
, mDirection(direction)
, mColor(color)
, mStyle(Forward)
, mNeedsShapeUpdate(true)
, mLine()
, mTriangle()
{
	assert(thickness > 0.f);

	setPosition(position);
}

void Arrow::setDirection(sf::Vector2f direction)
{
	mDirection = direction;
	mLength = length(direction);
	mNeedsShapeUpdate = true;
}

void Arrow::setDirection(float dirX, float dirY)
{
	setDirection(sf::Vector2f(dirX, dirY));
}

sf::Vector2f Arrow::getDirection() const
{
	return mDirection;
}

void Arrow::setThickness(float thickness)
{
	assert(thickness > 0.f);

	mThickness = thickness;
	mNeedsShapeUpdate = true;
}

float Arrow::getThickness() const
{
	return mThickness;
}

void Arrow::setColor(const sf::Color& color)
{
	mColor = color;

	mNeedsShapeUpdate = true;
}

sf::Color Arrow::getColor() const
{
	return mColor;
}

void Arrow::setStyle(Style style)
{
	mStyle = style;
}

Arrow::Style Arrow::getStyle() const
{
	return mStyle;
}

float Arrow::getTriangleHeight() const
{
	if (mStyle == Line)
		return 0.f;
	else
		return 4.f * mThickness;
}

void Arrow::adaptLine() const
{
	// Use circle to represent zero vector (every vector shorter than zeroVectorTolerance
	// is considered a zero vector)
	if (mLength <= zeroVectorTolerance)
	{
		mLine = Shapes::toConvexShape(sf::CircleShape(mThickness));
		mLine.setFillColor(mColor);
		mLine.move(-mThickness, -mThickness);
	}

	// If the line length is shorter than the triangle height, don't draw the line
	else if (mLength <= getTriangleHeight())
	{
		mLine = sf::ConvexShape();
	}

	// Normal arrow
	else
	{
		sf::Vector2f arrowDirection = (mLength - getTriangleHeight()) * unitVector(mDirection);

		mLine = Shapes::line(arrowDirection, mColor, getThickness());
	}
}

void Arrow::adaptTriangle() const
{
	// Don't draw triangle when arrow is too short or when arrow is actually a line
	if (mLength <= zeroVectorTolerance || mStyle == Line)
	{
		mTriangle = sf::ConvexShape();
	}

	// Draw normal triangle
	else
	{
		const float end   = std::max(mLength, getTriangleHeight());
		const float begin = end - getTriangleHeight();

		mTriangle.setFillColor(mColor);
		mTriangle.setRotation(polarAngle(mDirection));

		mTriangle.setPointCount(3);
		mTriangle.setPoint(0, sf::Vector2f(end, 0.f));
		mTriangle.setPoint(1, sf::Vector2f(begin,  1.5f * mThickness));
		mTriangle.setPoint(2, sf::Vector2f(begin, -1.5f * mThickness));
	}
}

void Arrow::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (mNeedsShapeUpdate)
	{
		adaptLine();
		adaptTriangle();
		mNeedsShapeUpdate = false;
	}

	states.transform *= getTransform();

	target.draw(mLine, states);
	target.draw(mTriangle, states);
}

float Arrow::getZeroVectorTolerance()
{
	return zeroVectorTolerance;
}

void Arrow::setZeroVectorTolerance(float tolerance)
{
	assert(tolerance >= 0.f);
	zeroVectorTolerance = tolerance;
}

} // namespace thor
