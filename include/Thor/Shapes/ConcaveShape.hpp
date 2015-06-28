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

/// @file
/// @brief Class thor::ConcaveShape

#ifndef THOR_CONCAVESHAPE_HPP
#define THOR_CONCAVESHAPE_HPP

#include <Thor/Config.hpp>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/ConvexShape.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/VertexArray.hpp>

#include <vector>


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

		/// @brief Sets the amount of points in the concave polygon.
		///
		void						setPointCount(std::size_t count);

		/// @brief Returns the amount of points in the concave polygon.
		///
		std::size_t					getPointCount() const;

		/// @brief Sets the position of a point.
		/// @param index Which point? Must be in [0, getPointCount()[
		/// @param position New point position in local coordinates.
		void						setPoint(std::size_t index, sf::Vector2f position);

		/// @brief Returns the position of a point.
		/// @param index Which point? Must be in [0, getPointCount()[
		sf::Vector2f				getPoint(std::size_t index) const;

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

		/// @brief Return untransformed bounding rectangle.
		///
		sf::FloatRect				getLocalBounds() const;

		/// @brief Return transformed bounding rectangle.
		/// @details This function does not necessarily yield the minimal bounding rect that spans all vertices. It transforms
		///  the local bounding rect, and then computes the bounding rect of that transformed bounding rect -- so for example, a
		///  rotated shape might not have the bounding rect that you expect. This behavior is consistent with SFML.
		sf::FloatRect				getGlobalBounds() const;


	// ---------------------------------------------------------------------------------------------------------------------------
	// Private types
	private:
		struct TriangleGenerator;


	// ---------------------------------------------------------------------------------------------------------------------------
	// Private member functions
	private:
		// Renders the shape to target.
		virtual void				draw(sf::RenderTarget& target, sf::RenderStates states) const;

		// Computes how the shape can be split up into convex triangles.
		void						ensureDecomposed() const;

		// Forms the outline out of the given edges.
		void						ensureOutlineUpdated() const;


	// ---------------------------------------------------------------------------------------------------------------------------
	// Private variables
	private:
		std::vector<sf::Vector2f>				mPoints;
		sf::Color								mFillColor;
		sf::Color								mOutlineColor;
		float									mOutlineThickness;

		mutable sf::VertexArray					mTriangleVertices;
		mutable std::vector<sf::ConvexShape>	mOutlineShapes;
		mutable sf::FloatRect					mLocalBounds;
		mutable bool							mNeedsDecomposition;
		mutable bool							mNeedsOutlineUpdate;
};

/// @}

} // namespace thor

#endif // THOR_CONCAVESHAPE_HPP
