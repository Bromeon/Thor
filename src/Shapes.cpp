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


#include <Thor/Multimedia/Shapes.hpp>
#include <Thor/Multimedia/ConcaveShape.hpp>
#include <Thor/Vectors/PolarVector.hpp>

#include <SFML/Graphics/ConvexShape.hpp>

#include <cassert>


namespace thor
{
namespace
{

	template <class Shape>
	void addPoint(Shape& shape, sf::Vector2f point)
	{
		const unsigned int size = shape.getPointCount();

		shape.setPointCount(size + 1);
		shape.setPoint(size, point);
	}

} // namespace
	
// ---------------------------------------------------------------------------------------------------------------------------
	

namespace Shapes
{

	sf::ConvexShape THOR_API toConvexShape(const sf::Shape& shape)
	{
		const unsigned int size = shape.getPointCount();

		sf::ConvexShape convexShape;

		// Adapt shape properties
		convexShape.setFillColor(shape.getFillColor());
		convexShape.setOutlineColor(shape.getOutlineColor());
		convexShape.setOutlineThickness(shape.getOutlineThickness());
		convexShape.setPointCount(size);

		// Adapt transform properties
		convexShape.setPosition(shape.getPosition());
		convexShape.setRotation(shape.getRotation());
		convexShape.setScale(shape.getScale());
		convexShape.setOrigin(shape.getOrigin());

		// Adapt texture properties
		convexShape.setTexture(shape.getTexture());
		convexShape.setTextureRect(shape.getTextureRect());

		for (unsigned int i = 0; i < size; ++i)
		{
			convexShape.setPoint(i, shape.getPoint(i));
		}

		return convexShape;
	}

	sf::ConvexShape line(sf::Vector2f direction, const sf::Color& color, float thickness)
	{
		sf::Vector2f perpendicular = unitVector(perpendicularVector(direction)) * 0.5f * thickness;

		sf::ConvexShape line;
		line.setFillColor(color);
		line.setPointCount(4);
		line.setPoint(0, -perpendicular);
		line.setPoint(1,  perpendicular);
		line.setPoint(2, direction + perpendicular);
		line.setPoint(3, direction - perpendicular);
		
		return line;
	}

	sf::ConvexShape roundedRect(sf::Vector2f size, float cornerRadius, 
		const sf::Color& fillColor, float outlineThickness, const sf::Color& outlineColor)
	{
		assert(outlineThickness >= 0.f);

		// Compute inner, rectangular rect and reach the outline by a radius vector.
		sf::ConvexShape shape;
		shape.setFillColor(fillColor);
		shape.setOutlineThickness(outlineThickness);
		shape.setOutlineColor(outlineColor);

		sf::Vector2f	currentCorner(size.x - cornerRadius, size.y - cornerRadius);
		PolarVector2f	radialVec(cornerRadius, 0.f);

		const unsigned int nbSegmentsPerCorner = 20;
		const float difference = 90.f / nbSegmentsPerCorner;
	
		// right lower rounded corner
		for (; radialVec.phi < 90.f; radialVec.phi += difference)
			addPoint(shape, currentCorner + sf::Vector2f(radialVec));
	
		// left lower rounded corner
		currentCorner.x = cornerRadius;
		for (radialVec.phi = 90.f; radialVec.phi < 180.f; radialVec.phi += difference)
			addPoint(shape, currentCorner + sf::Vector2f(radialVec));
	
		// left upper rounded corner
		currentCorner.y = cornerRadius;
		for (radialVec.phi = 180.f; radialVec.phi < 270.f; radialVec.phi += difference)
			addPoint(shape, currentCorner + sf::Vector2f(radialVec));
	
		// right upper rounded corner
		currentCorner.x = size.x - cornerRadius;
		for (radialVec.phi = 270.f; radialVec.phi < 360.f; radialVec.phi += difference)
			addPoint(shape, currentCorner + sf::Vector2f(radialVec));
				
		return shape;
	}

	sf::ConvexShape polygon(unsigned int nbPoints, float radius, const sf::Color& fillColor,
		float outlineThickness, const sf::Color& outlineColor)
	{
		assert(radius > 0.f);
		assert(outlineThickness >= 0.f);

		// Radial vector of the regular polygon
		PolarVector2f vector(radius, 0.f);
	
		sf::ConvexShape shape;
		shape.setFillColor(fillColor);
		shape.setOutlineThickness(outlineThickness);
		shape.setOutlineColor(outlineColor);
	
		// Add regularly distributed polygon points
		for (unsigned int points = 0; points < nbPoints; ++points)
		{
			vector.phi = 360.f * points / nbPoints;
		
			addPoint(shape, vector);
		}

		return shape;
	}
	
	sf::ConvexShape star(unsigned int nbStarPoints, float innerRadius, float outerRadius,
		const sf::Color& fillColor, float outlineThickness, const sf::Color& outlineColor)
	{
		assert(innerRadius > 0.f);
		assert(outerRadius > innerRadius);
		assert(outlineThickness >= 0.f);

		// Calculate points of the inner, regular polygon and the outer star points
		PolarVector2f      inner(innerRadius, 0.f);
		PolarVector2f      outer(outerRadius, 0.f);
	
		sf::ConvexShape shape;
		shape.setFillColor(fillColor);
		shape.setOutlineThickness(outlineThickness);
		shape.setOutlineColor(outlineColor);
		
		// Step around and alternately add inner and outer points
		for (unsigned int points = 0; points < nbStarPoints; ++points)
		{
			inner.phi = 360.f * points / nbStarPoints;
			outer.phi = inner.phi + 180.f / nbStarPoints;
		
			addPoint(shape, inner);
			addPoint(shape, outer);
		}

		return shape;
	}

	ConcaveShape pie(float radius, float filledAngle, const sf::Color& fillColor,
		float outlineThickness, const sf::Color& outlineColor)
	{
		assert(radius > 0.f);
	
		ConcaveShape shape;
		shape.setFillColor(fillColor);
		shape.setOutlineThickness(outlineThickness);
		shape.setOutlineColor(outlineColor);
	
		filledAngle = std::fmod(filledAngle, 360.f);

		// nbTotalPoints = the points that would be there at a full circle; nbActualPoints = the actual points
		const unsigned int nbTotalPoints = 40;
		const unsigned int nbActualPoints = static_cast<unsigned int>(filledAngle / 360.f * nbTotalPoints);

		// Add center point, unless the pie is full
		if (filledAngle != 0.f)
			addPoint(shape, sf::Vector2f(0.f, 0.f));
	
		PolarVector2f vector(radius, 0.f);
		for (unsigned int i = 0; i <= nbActualPoints; ++i)
		{
			vector.phi = 360.f * i / nbTotalPoints;
		
			addPoint(shape, vector);
		}
	
		return shape;
	}

} // namespace Shapes
} // namespace thor
