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
/// @brief Class thor::BigTexture

#ifndef THOR_BIGTEXTURE_HPP
#define THOR_BIGTEXTURE_HPP

#include <Thor/Config.hpp>

#include <SFML/Graphics/Texture.hpp>

#include <vector>
#include <string>


namespace sf
{

	class Sprite;

} // namespace sf


namespace thor
{

/// @addtogroup Graphics
/// @{

/// @brief Class for textures which are too big for sf::Texture.
/// @details sf::Texture cannot handle textures of which the size exceeds the hardware-given limit. Instead, you can use this
///  class, which has a very similar interface to sf::Texture. Internally, the class splits the texture into smaller parts which
///  are unproblematic for OpenGL. To display the texture on the screen, you can use thor::BigSprite.
class THOR_API BigTexture
{
	// ---------------------------------------------------------------------------------------------------------------------------
	// Public member functions
	public:
		/// @brief Default constructor
		///
									BigTexture();

		/// @brief Loads the texture from an image.
		/// @param image The sf::Image of which the pixels are loaded by the texture.
		bool						loadFromImage(const sf::Image& image);

		/// @brief Loads the texture from a file.
		/// @param filename Name of the file to load from.
		bool						loadFromFile(const std::string& filename);

		/// @brief Loads the texture from RAM.
		/// @param data Pointer to begin of the data.
		/// @param size Size of data in bytes.
		bool						loadFromMemory(const void* data, std::size_t size);

		/// @brief Loads the texture from a SFML input stream.
		/// @param stream Reference to sf::InputStream which loads the data.
		bool						loadFromStream(sf::InputStream& stream);

		/// @brief Returns the texture width and height in pixels.
		/// 
		sf::Vector2u				getSize() const;

		/// @brief Enables or disables SFML's smooth filter.
		/// @param smooth True to enable smoothing, false to disable it.
		void						setSmooth(bool smooth);

		/// @brief Tells whether SFML's smooth filter is enabled or not.
		/// 
		bool						isSmooth() const;


	// ---------------------------------------------------------------------------------------------------------------------------
	// Private member functions
	private:
		// Swaps *this with other.
		void						swap(BigTexture& other);

		// Fills the sprites according to the own 2D array of textures.
		// The return value is the size of everything, in world coordinates.
		sf::Vector2f				fillSprites(const sf::Color& color, std::vector<sf::Sprite>& out) const;


	// ---------------------------------------------------------------------------------------------------------------------------
	// Private variables
	private:
		std::vector<sf::Texture>	mTextures;
		sf::Vector2u				mTableSize;
		sf::Vector2u				mPixelSize;
		bool						mSmooth;


	// ---------------------------------------------------------------------------------------------------------------------------
	// Friends
	friend class BigSprite;
};

/// @}

} // namespace thor

#endif // THOR_BIGTEXTURE_HPP
