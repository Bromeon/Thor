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

#include <Thor/Shapes/ConcaveShape.hpp>
#include <Thor/Shapes/Shapes.hpp>
#include <Thor/Math/Triangulation.hpp>

#include <Aurora/Tools/ForEach.hpp>

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/CircleShape.hpp>

#include <algorithm>
#include <cassert>
#include <cmath>
#include <limits>


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
, mOutlineShapes()
, mLocalBounds()
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
, mOutlineShapes()
, mLocalBounds()
, mNeedsDecomposition(true)
, mNeedsOutlineUpdate(true)
{
	const std::size_t size = shape.getPointCount();

	setPointCount(size);
	for (std::size_t i = 0; i < size; ++i)
		setPoint(i, shape.getPoint(i));
}


void ConcaveShape::setPointCount(std::size_t count)
{
	mPoints.resize(count);

	mNeedsDecomposition = true;
	mNeedsOutlineUpdate = true;
}

std::size_t ConcaveShape::getPointCount() const
{
	return static_cast<std::size_t>(mPoints.size());
}

void ConcaveShape::setPoint(std::size_t index, sf::Vector2f position)
{
	mPoints[index] = position;

	mNeedsDecomposition = true;
	mNeedsOutlineUpdate = true;
}

sf::Vector2f ConcaveShape::getPoint(std::size_t index) const
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
	AURORA_FOREACH(const sf::ConvexShape& shape, mOutlineShapes)
		target.draw(shape, states);
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

	// Coordinates for bounding rect - initially largest possible negative rectangle - works also for 0 points
	sf::Vector2f boundsMin = std::numeric_limits<float>::max() * sf::Vector2f(1.f, 1.f);
	sf::Vector2f boundsMax = std::numeric_limits<float>::min() * sf::Vector2f(1.f, 1.f);

	const float radius = mOutlineThickness / 2.f;

	// Create outline based on SFML lines (rectangles) and circles
	mOutlineShapes.clear();
	for (std::size_t i = 0; i < mPoints.size(); ++i)
	{
		sf::Vector2f firstPos = mPoints[i];
		sf::Vector2f secondPos = mPoints[(i+1) % mPoints.size()];

		// Insert circles at the polygon points to round the outline off
		sf::CircleShape circle;
		circle.setPosition(firstPos - sf::Vector2f(radius, radius));
		circle.setRadius(radius);
		circle.setFillColor(mOutlineColor);

		// Create lines representing the edges
		sf::ConvexShape line = Shapes::line(secondPos - firstPos, mOutlineColor, mOutlineThickness);
		line.setPosition(firstPos);

		// Add shapes
		mOutlineShapes.push_back(Shapes::toConvexShape(circle));
		mOutlineShapes.push_back(line);

		// Update bounding rect
		boundsMin.x = std::min(boundsMin.x, firstPos.x);
		boundsMin.y = std::min(boundsMin.y, firstPos.y);
		boundsMax.x = std::max(boundsMax.x, firstPos.x);
		boundsMax.y = std::max(boundsMax.y, firstPos.y);
	}

	mLocalBounds = sf::FloatRect(boundsMin, boundsMax - boundsMin);
	mNeedsOutlineUpdate = false;
}

sf::FloatRect ConcaveShape::getLocalBounds() const
{
	// Empty shape has no valid bounding rect
	assert(!mPoints.empty());

	ensureOutlineUpdated();
	return mLocalBounds;
}

sf::FloatRect ConcaveShape::getGlobalBounds() const
{
	return getTransform().transformRect(getLocalBounds());
}

} // namespace thor
