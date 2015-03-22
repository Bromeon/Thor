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
/// @brief Class thor::BigSprite

#ifndef THOR_BIGSPRITE_HPP
#define THOR_BIGSPRITE_HPP

#include <Thor/Config.hpp>

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include <vector>


namespace sf
{

	class RenderTarget;
	class RenderStates;

} // namespace sf


namespace thor
{

class BigTexture;


/// @addtogroup Graphics
/// @{

/// @brief Sprite using big textures.
/// @details This class can be used to draw textures which are too big for sf::Texture and which are stored in thor::BigTexture.
///  The interface of thor::%BigSprite is very similar to sf::Sprite.
/// @n@n thor::%BigSprite inherits sf::Drawable and sf::Transformable, so it also contains getPosition(), getTransform(), draw()
///  etc. which are not listed here.
class THOR_API BigSprite : public sf::Drawable, public sf::Transformable
{
	// ---------------------------------------------------------------------------------------------------------------------------
	// Public member functions
	public:
		/// @brief Default constructor
		/// @details Creates an empty sprite that uses no texture.
									BigSprite();

		/// @brief Construct from texture
		/// @details Creates a sprite that uses the specified texture. The referenced texture must remain valid during the lifetime
		///  of this sprite.
		explicit					BigSprite(const BigTexture& texture);

		/// @brief Sets the texture which is used for this sprite.
		/// @details The referenced texture must remain valid during the lifetime of this sprite.
		void						setTexture(const BigTexture& texture);

		/// @brief Sets the sprite's color.
		/// 
		void						setColor(const sf::Color& color);

		/// @brief Returns the sprite's color.
		/// 
		sf::Color					getColor() const;

		/// @brief Returns the local bounding rect, starting at (0,0).
		/// 
		sf::FloatRect				getLocalBounds() const;

		/// @brief Returns the global bounding rect, with the current transform applied.
		/// 
		sf::FloatRect				getGlobalBounds() const;


	// ---------------------------------------------------------------------------------------------------------------------------
	// Private member functions
	private:
		// Overrides sf::Drawable::draw()
		virtual void				draw(sf::RenderTarget& target, sf::RenderStates states) const;


	// ---------------------------------------------------------------------------------------------------------------------------
	// Private variables
	private:
		std::vector<sf::Sprite>		mSplitSprites;
		sf::Vector2f				mSize;
		sf::Color					mColor;
};

/// @}

} // namespace thor

#endif // THOR_BIGSPRITE_HPP
