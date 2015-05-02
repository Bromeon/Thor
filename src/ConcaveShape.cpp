/////////////////////////////////////////////////////////////////////////////////
//
// Thor C++ Library
// Copyright (c) 2011-2015 Jan Haller
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

#include <Thor/Shapes/ConcaveShape.hpp>
#include <Thor/Math/Triangulation.hpp>

#include <Aurora/Tools/ForEach.hpp>

#include <SFML/Graphics/RenderTarget.hpp>

#include <algorithm>
#include <cassert>
#include <cmath>


// TODO: Possible optimization: Don't recompute everything if a single attribute such as fill color changes.

namespace thor
{

struct ConcaveShape::TriangleGenerator
{
	TriangleGenerator(sf::VertexArray& triangleVertices, const sf::Color& color)
	: triangleVertices(&triangleVertices)
	, color(color)
	{
		triangleVertices.clear();
	}

	// Fake dereferencing
	TriangleGenerator& operator* ()
	{
		return *this;
	}

	// Fake pre-increment
	TriangleGenerator& operator++ ()
	{
		return *this;
	}

	// Fake post-increment
	TriangleGenerator& operator++ (int)
	{
		return *this;
	}

	// Assignment from triangle
	TriangleGenerator& operator= (const Triangle<const sf::Vector2f>& triangle)
	{
		triangleVertices->append(sf::Vertex(triangle[0], color));
		triangleVertices->append(sf::Vertex(triangle[1], color));
		triangleVertices->append(sf::Vertex(triangle[2], color));

		return *this;
	}

	sf::VertexArray*	triangleVertices;
	sf::Color			color;
};

// ---------------------------------------------------------------------------------------------------------------------------


ConcaveShape::ConcaveShape()
: sf::Drawable()
, sf::Transformable()
, mPoints()
, mFillColor()
, mOutlineColor()
, mOutlineThickness(0.f)
, mTriangleVertices(sf::Triangles)
, mOutlineShape()
, mNeedsDecomposition(false)
, mNeedsOutlineUpdate(false)
{
}

ConcaveShape::ConcaveShape(const sf::Shape& shape)
: sf::Drawable()
, sf::Transformable(shape)
, mPoints()
, mFillColor(shape.getFillColor())
, mOutlineColor(shape.getOutlineColor())
, mOutlineThickness(shape.getOutlineThickness())
, mTriangleVertices(sf::Triangles)
, mOutlineShape()
, mNeedsDecomposition(true)
, mNeedsOutlineUpdate(true)
{
	const unsigned int size = shape.getPointCount();

	setPointCount(size);
	for (unsigned int i = 0; i < size; ++i)
		setPoint(i, shape.getPoint(i));
}


void ConcaveShape::setPointCount(unsigned int count)
{
	mPoints.resize(count);

	mNeedsDecomposition = true;
	mNeedsOutlineUpdate = true;
}

unsigned int ConcaveShape::getPointCount() const
{
	return static_cast<unsigned int>(mPoints.size());
}

void ConcaveShape::setPoint(unsigned int index, sf::Vector2f position)
{
	mPoints[index] = position;

	mNeedsDecomposition = true;
	mNeedsOutlineUpdate = true;
}

sf::Vector2f ConcaveShape::getPoint(unsigned int index) const
{
	return mPoints[index];
}

void ConcaveShape::setFillColor(const sf::Color& fillColor)
{
	mFillColor = fillColor;
	mNeedsDecomposition = true;
}

sf::Color ConcaveShape::getFillColor() const
{
	return mFillColor;
}

void ConcaveShape::setOutlineColor(const sf::Color& outlineColor)
{
	mOutlineColor = outlineColor;
	mNeedsOutlineUpdate = true;
}

sf::Color ConcaveShape::getOutlineColor() const
{
	return mOutlineColor;
}

void ConcaveShape::setOutlineThickness(float outlineThickness)
{
	assert(outlineThickness >= 0.f);

	mOutlineThickness = outlineThickness;
	mNeedsOutlineUpdate = true;
}

float ConcaveShape::getOutlineThickness() const
{
	return mOutlineThickness;
}

void ConcaveShape::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	// One or zero points aren't rendered
	if (mPoints.size() <= 1)
		return;

	// Update cache if needed
	ensureDecomposed();
	ensureOutlineUpdated();

	// Combine transforms
	states.transform *= getTransform();

	// Draw all triangles and the outline
	target.draw(mTriangleVertices, states);
	target.draw(mOutlineShape, states);
}

void ConcaveShape::ensureDecomposed() const
{
	if (!mNeedsDecomposition)
		return;

	// Split the concave polygon into convex triangles
	triangulatePolygon(mPoints.begin(), mPoints.end(), TriangleGenerator(mTriangleVertices, mFillColor));
	mNeedsDecomposition = false;
}

void ConcaveShape::ensureOutlineUpdated() const
{
	// If no outline is visible, don't create one
	// The optimization for mOutlineThickness == 0.f can't be used, as the outline is needed for bounds computation
	if (!mNeedsOutlineUpdate)
		return;

	// Reuse a SFML convex shape for the concave outline; fill it with transparent color
	mOutlineShape.setPointCount(mPoints.size());
	mOutlineShape.setFillColor(sf::Color::Transparent);
	mOutlineShape.setOutlineColor(mOutlineColor);
	mOutlineShape.setOutlineThickness(mOutlineThickness);

	for (unsigned int i = 0; i < mPoints.size(); ++i)
		mOutlineShape.setPoint(i, mPoints[i]);

	mNeedsOutlineUpdate = false;
}

sf::FloatRect ConcaveShape::getLocalBounds() const
{
	ensureOutlineUpdated();
	return mOutlineShape.getLocalBounds();
}

sf::FloatRect ConcaveShape::getGlobalBounds() const
{
	return getTransform().transformRect(getLocalBounds());
}

} // namespace thor
