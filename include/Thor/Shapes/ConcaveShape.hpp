/////////////////////////////////////////////////////////////////////////////////
//
// Thor C++ Library
// Copyright (c) 2011-2013 Jan Haller
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

/// @file
/// @brief Class thor::ConcaveShape

#ifndef THOR_CONCAVESHAPE_HPP
#define THOR_CONCAVESHAPE_HPP

#include <Thor/Math/TriangulationFigures.hpp>
#include <Thor/Config.hpp>

#include <Aurora/SmartPtr/CopiedPtr.hpp>
#include <Aurora/Tools/Swap.hpp>

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Color.hpp>

#include <vector>


namespace sf
{

	class Shape;

} // namespace sf


namespace thor
{

/// @addtogroup Shapes
/// @{

/// @brief Concave shape class
/// @details This class has an interface and functionality similar to sf::ConvexShape, but is additionally able to work
///  with shapes that are concave. It inherits the sf::Drawable and sf::Transformable classes.
class THOR_API ConcaveShape : public sf::Drawable, public sf::Transformable
{
	// ---------------------------------------------------------------------------------------------------------------------------
	// Public member functions
	public:
		/// @brief Creates an empty shape.
		/// @details You can use setPointCount() and SetPoint() to build your own concave shape.
									ConcaveShape();
							
		/// @brief Implicit constructor that adapts an existing sf::Shape.
		/// @details The concave shape should look as similar as possible to the original convex shape.
									ConcaveShape(const sf::Shape& shape);

		/// @brief Exchanges the content of two instances in O(1).
		///
		void						swap(ConcaveShape& other);

		
		/// @brief Sets the amount of points in the concave polygon.
		///
		void						setPointCount(unsigned int count);

		/// @brief Returns the amount of points in the concave polygon.
		///
		unsigned int				getPointCount() const;

		/// @brief Sets the position of a point.
		/// @param index Which point? Must be in [0, getPointCount()[
		/// @param position New point position in local coordinates.
		void						setPoint(unsigned int index, sf::Vector2f position);

		/// @brief Returns the position of a point.
		/// @param index Which point? Must be in [0, getPointCount()[
		sf::Vector2f				getPoint(unsigned int index) const;

		/// @brief Sets the polygon's fill color.
		/// 
		void						setFillColor(const sf::Color& fillColor);

		/// @brief Sets the polygon's outline color.
		/// 
		void						setOutlineColor(const sf::Color& outlineColor);

		/// @brief Returns the polygon's fill color.
		/// 
		sf::Color					getFillColor() const;

		/// @brief Returns the polygon's outline color.
		/// 
		sf::Color					getOutlineColor() const;

		/// @brief sets the thickness of the shape's outline (0.f by default).
		/// @details @a outlineThickness must be greater or equal to zero. If it is zero, no outline is visible.
		void						setOutlineThickness(float outlineThickness);
		
		/// @brief Returns the shape's outline thickness.
		///
		float						getOutlineThickness() const;
		

	// ---------------------------------------------------------------------------------------------------------------------------
	// Private types
	private:	 		
		// Container typedefs
		typedef std::vector< aurora::CopiedPtr<sf::Shape> >	ShapeContainer;
		typedef std::vector< sf::Vector2f >					PointContainer;
		typedef std::vector< Edge<const sf::Vector2f> >		EdgeContainer;

		struct TriangleGenerator;


	// ---------------------------------------------------------------------------------------------------------------------------
	// Private member functions
	private:
		// Renders the shape to target.
		virtual void				draw(sf::RenderTarget& target, sf::RenderStates states) const;
		
		// Computes how the shape can be split up into convex triangles.
		void						decompose() const;
				
		// Forms the outline out of the given edges.
		void						formOutline() const;


	// ---------------------------------------------------------------------------------------------------------------------------
	// Private variables
	private:
		PointContainer				mPoints;
		sf::Color					mFillColor;
		sf::Color					mOutlineColor;
		float						mOutlineThickness;

		mutable EdgeContainer		mEdges;
		mutable ShapeContainer		mTriangleShapes;
		mutable ShapeContainer		mEdgeShapes;
		mutable bool				mNeedsTriangleUpdate;
		mutable bool				mNeedsEdgeUpdate;
};

/// @relates ConcaveShape
/// @brief Exchanges the contents of two concave shapes.
AURORA_GLOBAL_SWAP(ConcaveShape)

/// @}

} // namespace thor

#endif // THOR_CONCAVESHAPE_HPP
