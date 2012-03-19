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

/// @file
/// @brief Mixin-classes for 2D object attributes (position, rotation, scale, ...)

#ifndef THOR_OBJECT2D_HPP
#define THOR_OBJECT2D_HPP

#include <Thor/Config.hpp>

#include <SFML/System/Vector2.hpp>


namespace thor
{

/// @addtogroup Geometry
/// @{

/// @brief Functionality class for a 2D position.
/// @details Stores a position vector and provides methods to access it.
class THOR_API Positionable
{
	// ---------------------------------------------------------------------------------------------------------------------------
	// Public member functions
	public:
		/// @brief (Default) Constructor
		///
		explicit					Positionable(sf::Vector2f position = sf::Vector2f(0.f, 0.f));

		/// @brief Sets the object's position.
		///
		void						setPosition(sf::Vector2f position);
		
		/// @brief Sets the object's position.
		///
		void						setPosition(float x, float y);

		/// @brief Moves the object by a given offset.
		///
		void						move(sf::Vector2f offset);

		/// @brief Moves the object by a given offset.
		///
		void						move(float offsetX, float offsetY);

		/// @brief Returns the object's position.
		///
		sf::Vector2f				getPosition() const;


	// ---------------------------------------------------------------------------------------------------------------------------
	// Private variables
	private:
		sf::Vector2f				mPosition;
};


/// @brief Functionality class for a 2D rotation.
/// @details Stores a rotation angle and provides methods to access it.
class THOR_API Rotatable
{
	// ---------------------------------------------------------------------------------------------------------------------------
	// Public member functions
	public:	
		/// @brief (Default) Constructor
		///
		explicit					Rotatable(float rotation = 0.f);

		/// @brief sets the object's rotation (in degrees).
		///
		void						setRotation(float rotation);

		/// @brief Add a given angle to the object's rotation (in degrees).
		///
		void						rotate(float angle);

		/// @brief Returns the object's rotation (in degrees).
		///
		float						getRotation() const;


	// ---------------------------------------------------------------------------------------------------------------------------
	// Private variables
	private:	
		float						mRotation;
};


/// @brief Functionality class for a 2D scale.
/// @details Stores a scale vector and provides methods to access it.
class THOR_API Scalable
{
	// ---------------------------------------------------------------------------------------------------------------------------
	// Public member functions
	public:	
		/// @brief (Default) Constructor
		///
		explicit					Scalable(sf::Vector2f scale = sf::Vector2f(1.f, 1.f));

		/// @brief Sets the object's absolute scale.
		///
		void						setScale(sf::Vector2f scale);

		/// @brief Sets the object's absolute scale.
		///
		void						setScale(float scaleX, float scaleY);

		/// @brief Multiplies the current scale with given factors.
		///
		void						scale(sf::Vector2f factor);

		/// @brief Multiplies the current scale with given factors.
		///
		void						scale(float factorX, float factorY);

		/// @brief Returns the object's absolute scale.
		///
		sf::Vector2f				getScale() const;


	// ---------------------------------------------------------------------------------------------------------------------------
	// Private variables
	private:
		sf::Vector2f				mScale;
};

/// @}

} // namespace thor

#endif // THOR_OBJECT2D_HPP
