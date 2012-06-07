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

#include <Thor/Multimedia/ConcaveShape.hpp>
#include <Thor/Multimedia/Shapes.hpp>
#include <Thor/Math/Triangulation.hpp>
#include <Thor/Vectors/PolarVector.hpp>

#include <Aurora/Tools/ForEach.hpp>
#include <Aurora/SmartPtr/CopiedPtr.hpp>

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/ConvexShape.hpp>
#include <SFML/Graphics/CircleShape.hpp>

#include <algorithm>
#include <cassert>
#include <cmath>


namespace thor
{

struct ConcaveShape::TriangleGenerator
{
	TriangleGenerator(ShapeContainer& triangles, const sf::Color& color)
	: triangles(triangles)
	, color(color)
	{
		triangles.clear();
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
		aurora::CopiedPtr<sf::ConvexShape> shape(new sf::ConvexShape());
		shape->setPointCount(3);
		shape->setFillColor(color);
		
		for (unsigned int i = 0; i < 3; ++i)
			shape->setPoint(i, triangle[i]);
		
		triangles.push_back(std::move(shape));
		return *this;
	}

	ShapeContainer&	triangles;
	sf::Color		color;
};

// ---------------------------------------------------------------------------------------------------------------------------


ConcaveShape::ConcaveShape()
: sf::Drawable()
, sf::Transformable()
, mPoints()
, mFillColor()
, mOutlineColor()
, mOutlineThickness(0.f)
, mEdges()
, mTriangleShapes()
, mEdgeShapes()
, mNeedsTriangleUpdate(false)
, mNeedsEdgeUpdate(false)
{
}

ConcaveShape::ConcaveShape(const sf::Shape& shape)
: sf::Drawable()
, sf::Transformable(shape)
, mPoints()
, mFillColor(shape.getFillColor())
, mOutlineColor(shape.getOutlineColor())
, mOutlineThickness(shape.getOutlineThickness())
, mEdges()
, mTriangleShapes()
, mEdgeShapes()
, mNeedsTriangleUpdate(true)
, mNeedsEdgeUpdate(true)
{
	const unsigned int size = shape.getPointCount();

	setPointCount(size);
	for (unsigned int i = 0; i < size; ++i)
		setPoint(i, shape.getPoint(i));
}


void ConcaveShape::setPointCount(unsigned int count)
{
	mPoints.resize(count);
}

unsigned int ConcaveShape::getPointCount() const
{
	return mPoints.size();
}

void ConcaveShape::setPoint(unsigned int index, sf::Vector2f position)
{
	mPoints[index] = position;

	mNeedsTriangleUpdate = true;
	mNeedsEdgeUpdate = true;
}

sf::Vector2f ConcaveShape::getPoint(unsigned int index) const
{
	return mPoints[index];
}

void ConcaveShape::setFillColor(const sf::Color& fillColor)
{
	mFillColor = fillColor;
	mNeedsTriangleUpdate = true;
}

sf::Color ConcaveShape::getFillColor() const
{
	return mFillColor;
}

void ConcaveShape::setOutlineColor(const sf::Color& outlineColor)
{
	mOutlineColor = outlineColor;
	mNeedsEdgeUpdate = true;
}

sf::Color ConcaveShape::getOutlineColor() const
{
	return mOutlineColor;
}

void ConcaveShape::setOutlineThickness(float outlineThickness)
{
	assert(outlineThickness >= 0.f);

	mOutlineThickness = outlineThickness;	
	mNeedsEdgeUpdate = true;
}
		
float ConcaveShape::getOutlineThickness() const
{
	return mOutlineThickness;
}

void ConcaveShape::swap(ConcaveShape& other)
{
	using std::swap;

	swap(static_cast<sf::Transformable&>(*this), static_cast<sf::Transformable&>(other));

	swap(mPoints,              other.mPoints);
	swap(mFillColor,           other.mFillColor);
	swap(mOutlineColor,		   other.mOutlineColor);
	swap(mOutlineThickness,    other.mOutlineThickness);
	swap(mEdges,               other.mEdges);
	swap(mTriangleShapes,      other.mTriangleShapes);
	swap(mEdgeShapes,          other.mEdgeShapes);
	swap(mNeedsTriangleUpdate, other.mNeedsTriangleUpdate);
	swap(mNeedsEdgeUpdate,     other.mNeedsEdgeUpdate);
}

void ConcaveShape::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	// One or zero points aren't rendered
	if (mPoints.size() <= 1)
		return;
	
	// Batch logics
	if (mNeedsEdgeUpdate || mNeedsTriangleUpdate)
	{		
		if (mNeedsTriangleUpdate)
			decompose();

		if (mNeedsEdgeUpdate)
			formOutline();

		mNeedsEdgeUpdate = false;
		mNeedsTriangleUpdate = false;
	}

	// Combine transforms
	states.transform *= getTransform();
	
	// Draw all points
	AURORA_CITR_FOREACH(ShapeContainer, mTriangleShapes, itr)
		target.draw(**itr, states);
	
	// Draw all edges at the boundary
	AURORA_CITR_FOREACH(ShapeContainer, mEdgeShapes, itr)
		target.draw(**itr, states);
}

void ConcaveShape::decompose() const
{
	mEdges.clear();

	// Split the concave polygon into convex triangles that can be represented by sf::ConvexShape
	triangulatePolygon(mPoints.begin(), mPoints.end(), TriangleGenerator(mTriangleShapes, mFillColor), std::back_inserter(mEdges));
}

void ConcaveShape::formOutline() const
{
	// If no outline is visible, don't create one
	if (mOutlineThickness == 0.f)
		return;

	// Create graphic edges	
	AURORA_CITR_FOREACH(EdgeContainer, mEdges, itr)
	{
		sf::Vector2f firstPos = (*itr)[0];
		sf::Vector2f secondPos = (*itr)[1];
		const float radius = mOutlineThickness / 2.f;

		// Insert circles at the polygon points to round the outline off
		aurora::CopiedPtr<sf::CircleShape> circle(new sf::CircleShape());
		circle->setPosition(firstPos - sf::Vector2f(radius, radius));
		circle->setRadius(radius);
		circle->setFillColor(mOutlineColor);

		// Create lines representing the edges
		sf::ConvexShape line = Shapes::line(secondPos - firstPos, mOutlineColor, mOutlineThickness);
		line.setPosition(firstPos);

		// Add shapes
		mEdgeShapes.push_back( std::move(circle) );
		mEdgeShapes.push_back( aurora::copied(new sf::ConvexShape(line)) );
	}
}

} // namespace thor
